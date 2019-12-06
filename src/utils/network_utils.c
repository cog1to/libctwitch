#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/network_utils.h"
#include "utils/string_utils.h"
#include "utils/parser.h"
#include "json/json.h"

/** cURL helpers **/

size_t twitch_writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t append_size = size*nmemb;
  string_append(ptr, append_size, s);
  return append_size;
}

const char *url_encode(const char *string) {
  // Initialize curl.
  CURL *curl;
  curl = curl_easy_init();

  // Perform URL escaping.
  const char *escaped = curl_easy_escape(curl, string, strlen(string));

  curl_easy_cleanup(curl);
  return escaped;
}

CURLcode twitch_v5_get(const char *client_id, const char *url, string_t *output) {
  // Initialize curl.
  CURL *curl;
  curl = curl_easy_init();

  // Headers list.
  struct curl_slist *headers = NULL;

  // Basic headers.
  headers = curl_slist_append(headers, "Accept: application/vnd.twitchtv.v5+json");

  // Construct client ID header.
  string_t *client_id_header = string_init_with_value("Client-ID: ");
  string_append((void *)client_id, strlen(client_id), client_id_header);
  headers = curl_slist_append(headers, client_id_header->ptr);

  // Set headers.
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  // Set URL.
  curl_easy_setopt(curl, CURLOPT_URL, url);

  // Setup output buffer.
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitch_writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

  // Perform curl operation.
  CURLcode code = curl_easy_perform(curl);

  // Cleanup.
  curl_slist_free_all(headers);
  string_free(client_id_header);
  curl_easy_cleanup(curl);

  return code;
}

json_value *twitch_v5_get_json(const char *client_id, const char *url) {
  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url, output);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse JSON.");
    exit(EXIT_FAILURE);
  }

  return value;
}

/** Helpers **/

void append_paging_params(string_t *url, int limit, int offset, bool is_first_param) {
  char buffer[64];
  sprintf(buffer, "%slimit=%i&offset=%i", is_first_param ? "?" : "&", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);
}

void append_cursor(string_t *url, const char *cursor, bool is_first_param) {
  if (cursor == NULL) {
    return;
  }

  char buffer[64];
  sprintf(buffer, "%scursor=%s", is_first_param ? "?" : "&", cursor);
  string_append((void *)buffer, strlen(buffer), url);
}

void **get_page(const char *client_id, page_url_builder builder, void *params, int limit, int offset, const char *values_key, parser_func parser, int *size, int *total) {
  string_t *url = builder(params, limit, offset);
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  void **elements = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, values_key) == 0) {
      json_value *elements_value = value->u.object.values[x].value;
      int elements_length = elements_value->u.array.length;
      if (elements_length == 0) {
        *size = 0;
        break;
      }

      elements = parse_json_array(elements_value, size, parser);
    } else if (strcmp(value->u.object.values[x].name, "_total") == 0) {
      *total = value->u.object.values[x].value->u.integer;
    }
  }

  json_value_free(value);
  return elements;
}

void **get_cursored_page(const char *client_id, cursor_page_url_builder builder, void *params, int limit, const char *cursor, const char *values_key, parser_func parser, int *size, char **next_cursor) {
  string_t *url = builder(params, limit, cursor);
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  void **elements = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, values_key) == 0) {
      json_value *elements_value = value->u.object.values[x].value;
      int elements_length = elements_value->u.array.length;
      if (elements_length == 0) {
        *size = 0;
        break;
      }

      elements = parse_json_array(elements_value, size, parser);
    } else if (strcmp(value->u.object.values[x].name, "_cursor") == 0) {
      (*next_cursor) = immutable_string_copy(value->u.object.values[x].value->u.string.ptr);
    }
  }

  json_value_free(value);
  return elements;
}

void **get_all_pages(const char *client_id, page_url_builder builder, void *params, const char *values_key, parser_func parser, bool ignore_totals, int *size) {
  const int PAGE_SIZE = 20;

  int count = 0;
  int total = 0;
  int offset = 0;
  void **elements = NULL;

  do {
    void **page = get_page(client_id, builder, params, PAGE_SIZE, offset, values_key, parser, &count, &total);

    // Don't do anything if there are 0 items returned. It should mean we're at the end of the list.
    if (count == 0) {
      if (offset + count < total || total == 0) {
        total = (offset + count);
      }
      break;
    }

    // (Re)allocate memory to store next page.
    if (offset == 0) {
      elements = malloc(sizeof(void *) * count);
    } else {
      elements = realloc(elements, sizeof(void *) * (offset + count));
    }

    if (elements == NULL) {
      fprintf(stderr, "Failed to allocate memory for next page.");
      exit(EXIT_FAILURE);
    }

    // Copy page's content to the overall storage.
    memcpy(&elements[offset], page, sizeof(void *) * count);

    // Offset to the next page.
    offset += count;

    // Free current page data.
    free(page);
  } while ((ignore_totals && count > 0) || (offset < total));

  // Return the whole list.
  *size = total;
  return elements;
}

void **get_all_cursored_pages(const char *client_id, cursor_page_url_builder builder, void *params, const char *values_key, parser_func parser, int *size) {
  const int PAGE_SIZE = 20;

  int count = 0;
  int total = 0;
  void **elements = NULL;
  char *cursor = NULL;
  char *next_cursor = NULL;

  do {
    void **page = get_cursored_page(client_id, builder, params, PAGE_SIZE, cursor, values_key, parser, &count, &next_cursor);

    // Don't do anything if there are 0 items returned. It should mean we're at the end of the list.
    if (count == 0 && next_cursor == NULL) {
      break;
    }

    // (Re)allocate memory to store next page.
    if (total == 0) {
      elements = malloc(sizeof(void *) * count);
    } else {
      elements = realloc(elements, sizeof(void *) * (total + count));
    }

    if (elements == NULL) {
      fprintf(stderr, "Failed to allocate memory for next page.");
      exit(EXIT_FAILURE);
    }

    // Copy page's content to the overall storage.
    memcpy(&elements[total], page, sizeof(void *) * count);

    // Update total count.
    total += count;

    // Free current page data.
    free(page);

    // Update cursor, if needed.
    if (next_cursor != NULL) {
      if (cursor != NULL) {
        free(cursor);
      }
      cursor = immutable_string_copy(next_cursor);
      free(next_cursor);
      next_cursor = NULL;
    }
  } while (count > 0);

  // Free the cursor memory.
  if (cursor != NULL) {
    free(cursor);
  }

  // Return the whole list.
  *size = total;
  return elements;
}

