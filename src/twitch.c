#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <curl/curl.h>
#include <wchar.h>

#include "string_utils.h"
#include "json.h"
#include "data.h"
#include "parser.h"

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

/** Helpers **/

typedef string_t *(*page_url_builder)(void *);

typedef struct {
  const char *user_id;
  const char *direction;
  const char *sortby;
} follows_params;

string_t *user_follows_url_builder(void *params) {
  char buffer[128];

  follows_params *fparams = (follows_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)fparams->user_id, strlen(fparams->user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);

  // Append direction.
  if (fparams->direction != NULL) {
    sprintf(buffer, "&direction=%s", fparams->direction);
    string_append(buffer, strlen(buffer), url);
  }

  // Append sorting order.
  if (fparams->sortby != NULL) {
    sprintf(buffer, "&sortby=%s", fparams->sortby);
    string_append(buffer, strlen(buffer), url);
  }

  return url;
}

typedef void *(*parser_func)(json_value *);

void **get_page(const char *client_id, page_url_builder builder, void *params, int limit, int offset, const char *values_key, parser_func parser, int *size, int *total) {
  // Build the URL.
  string_t *url = builder(params);

  // Append paging params.
  char buffer[64];
  sprintf(buffer, "?limit=%i&offset=%i", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

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

void **get_all_pages(const char *client_id, page_url_builder builder, void *params, const char *values_key, parser_func parser, int *size) {
  const int PAGE_SIZE = 20;

  int count = 0;
  int total = 0;
  int offset = 0;
  void **elements = NULL;

  do {
    void **page = get_page(client_id, builder, params, PAGE_SIZE, offset, values_key, parser, &count, &total);

    // Don't do anything if there are 0 follows.
    if (count == 0) {
      if (offset + count < total) {
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
  } while (offset < total);

  // Return the whole list.
  *size = total;
  return elements;
}

/** API **/

twitch_user *twitch_v5_get_user(const char *client_id, const char *id) {
  twitch_user *user = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)id, strlen(id), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse users JSON.");
    exit(EXIT_FAILURE);
  }

  if (value->type != json_object) {
    fprintf(stderr, "Wrong JSON type of returned value.");
    exit(EXIT_FAILURE);
  }

  user = parse_user(value);
  json_value_free(value);

  return user;
}

twitch_user *twitch_v5_get_user_by_username(const char *client_id, const char *username) {
  // Output value.
  twitch_user *user = NULL; //

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users?login=");
  string_append((void *)username, strlen(username), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse users JSON.");
    exit(EXIT_FAILURE);
  }

  // Extract the relevant fields.
  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      json_value *user_object = users_value->u.array.values[0];
      user = parse_user(user_object);
    }
  }

  json_value_free(value);
  return user;
}

twitch_user **twitch_v5_get_users(const char *client_id, int usernames_count, const char **usernames, int *total) {
  char buffer[2048];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users");

  // Append usernames.
  if (usernames_count > 0) {
    string_t *usernames_string = string_joined(usernames_count, usernames, ",");
    sprintf(buffer, "?login=%s", usernames_string->ptr);
    string_append((void *)buffer, strlen(buffer), url);
    string_free(usernames_string);
  }

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse channels JSON.");
    exit(EXIT_FAILURE);
  }

  // Extract the relevant fields.
  twitch_user **users = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      users = (twitch_user **)parse_json_array(users_value, total, &parse_user);
    }
  }

  json_value_free(value);
  return users;
}

twitch_follow *twitch_v5_check_user_follow(const char *client_id, const char *user_id, const char *channel_id) {
  // Output value.
  twitch_follow *follow = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)user_id, strlen(user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);
  string_append((void *)channel_id, strlen(channel_id), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    return NULL;
  }

  follow = parse_follow(value);
  json_value_free(value);

  return follow;
}

twitch_follow **twitch_v5_get_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int limit, int offset, int *size, int *total) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_page(client_id, &user_follows_url_builder, (void *)&params, limit, offset, "follows", &parse_follow, size, total);
  return follows;
}

twitch_follow** twitch_v5_get_all_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int *size) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_all_pages(client_id, &user_follows_url_builder, (void *)&params, "follows", &parse_follow, size);
  return follows;
}

twitch_stream **twitch_v5_get_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int limit, int offset, int *size, int *total) {
  char buffer[2048];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/");

  // Append paging params.
  sprintf(buffer, "?limit=%i&offset=%i", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);

  // Append game.
  if (game != NULL) {
    sprintf(buffer, "&game=%s", game);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Append channel ids.
  if (channel_ids_count > 0) {
    string_t *channels = string_joined(channel_ids_count, channel_ids, ",");
    sprintf(buffer, "&channel=%s", channels->ptr);
    string_append((void *)buffer, strlen(buffer), url);
    string_free(channels);
  }

  // Append stream type.
  if (stream_type != NULL) {
    sprintf(buffer, "&stream_type=%s", stream_type);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Append language.
  if (language != NULL) {
    sprintf(buffer, "&language=%s", language);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse channels JSON.");
    exit(EXIT_FAILURE);
  }

  // Extract the relevant fields.
  twitch_stream **streams = NULL;

  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "streams") == 0) {
      json_value *streams_value = value->u.object.values[x].value;
      int streams_length = streams_value->u.array.length;
      if (streams_length == 0) {
        break;
      }

      streams = (twitch_stream **)parse_json_array(streams_value, size, &parse_stream);
    } else if (strcmp(value->u.object.values[x].name, "_total") == 0) {
      *total = value->u.object.values[x].value->u.integer;
    }
  }

  json_value_free(value);

  return streams;
}

twitch_stream** twitch_v5_get_all_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int *size) {
  const int PAGE_SIZE = 20;

  int stream_count = 0;
  int total = 0;
  int offset = 0;

  twitch_stream **streams = NULL;

  do {
    twitch_stream **page = twitch_v5_get_streams(client_id, channel_ids_count, channel_ids, game, stream_type, language, PAGE_SIZE, offset, &stream_count, &total);

    // Don't do anything if there are 0 streams.
    if (stream_count == 0) {
      break;
    }

    // (Re)allocate memory to store next page.
    if (offset == 0) {
      streams = malloc(sizeof(void *) * stream_count);
    } else {
      streams = realloc(streams, sizeof(void *) * (stream_count + offset));
    }

    if (streams == NULL) {
      fprintf(stderr, "Failed to allocate memory for next streams page.");
      exit(EXIT_FAILURE);
    }

    // Copy page's content to the overall storage.
    memcpy(&streams[offset], page, sizeof(void *) * stream_count);

    // Offset to the next page.
    offset += stream_count;

    // Free current page data.
    free(page);
  } while (offset < total);

  // Return the whole list.
  *size = total;
  return streams;
}

twitch_stream *twitch_v5_get_stream_by_user(const char *client_id, const char *channel_id, const char *stream_type) {
  twitch_stream *stream = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/");

  // Append Channel ID params.
  string_append((void *)channel_id, strlen(channel_id), url);

  // Append type.
  if (stream_type != NULL) {
    char buffer[24];
    sprintf(buffer, "?stream_type=%s", stream_type);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse channels JSON.");
    exit(EXIT_FAILURE);
  }

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "stream") == 0) {
      json_value *stream_value = value->u.object.values[prop_ind].value;
      if (stream_value->type == json_null) {
        break;
      }

      stream = (twitch_stream *)parse_stream(stream_value);
    }
  }

  json_value_free(value);
  return stream;
}

twitch_summary *twitch_v5_get_summary(const char *client_id, const char *game) {
  twitch_summary *summary = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/summary");

  // Append game name.
  if (game != NULL) {
    char buffer[256];
    sprintf(buffer, "?game=%s", game);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse summary JSON.");
    exit(EXIT_FAILURE);
  }

  summary = (twitch_summary *)parse_summary(value);

  json_value_free(value);
  return summary;
}

twitch_featured_stream **twitch_v5_get_featured_streams(const char *client_id, int limit, int offset, int *size) {
  char buffer[128];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/featured");

  // Append paging params.
  sprintf(buffer, "?limit=%i&offset=%i", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse featured streams JSON.");
    exit(EXIT_FAILURE);
  }

  // Extract the relevant fields.
  twitch_featured_stream **streams = NULL;

  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "featured") == 0) {
      json_value *streams_value = value->u.object.values[x].value;
      int streams_length = streams_value->u.array.length;
      if (streams_length == 0) {
        *size = 0;
        break;
      }

      streams = (twitch_featured_stream **)parse_json_array(streams_value, size, &parse_featured_stream);
    }
  }

  json_value_free(value);
  return streams;
}

twitch_featured_stream **twitch_v5_get_all_featured_streams(const char *client_id, int *size) {
  const int PAGE_SIZE = 20;

  int stream_count = 0;
  int total = 0;
  int offset = 0;

  twitch_featured_stream **streams = NULL;

  do {
    twitch_featured_stream **page = twitch_v5_get_featured_streams(client_id, PAGE_SIZE, offset, &stream_count);

    // Don't do anything if there are 0 items.
    if (stream_count == 0) {
      break;
    }

    // (Re)allocate memory to store next page.
    if (offset == 0) {
      streams = malloc(sizeof(void *) * stream_count);
    } else {
      streams = realloc(streams, sizeof(void *) * (offset + stream_count));
    }

    if (streams == NULL) {
      fprintf(stderr, "Failed to allocate memory for next featured streams page.");
      exit(EXIT_FAILURE);
    }

    // Copy page's content to the overall storage.
    memcpy(&streams[offset], page, sizeof(void *) * stream_count);

    // Offset to the next page.
    offset += stream_count;

    // Increase total count.
    total += stream_count;

    // Free current page data.
    free(page);
  } while (stream_count == PAGE_SIZE);

  // Return the whole list.
  *size = total;
  return streams;
}

twitch_channel **twitch_v5_search_channels(const char *client_id, const char *query, int limit, int offset, int *size, int *total) {
  char buffer[128];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/search/channels");

  // Append query.
  const char *urlencoded_query = url_encode(query);
  string_append((void *)"?query=", strlen("?query="), url);
  string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

  // Append paging params.
  sprintf(buffer, "&limit=%i&offset=%i", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);

  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url->ptr, output);
  string_free(url);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse channels JSON.");
    exit(EXIT_FAILURE);
  }

  // Extract the relevant fields.
  twitch_channel **channels = NULL;

  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "channels") == 0) {
      json_value *channels_value = value->u.object.values[x].value;
      int channels_length = channels_value->u.array.length;
      if (channels_length == 0) {
        *size = 0;
        break;
      }

      channels = (twitch_channel **)parse_json_array(channels_value, size, &parse_channel);
    } else if (strcmp(value->u.object.values[x].name, "_total") == 0) {
      *total = value->u.object.values[x].value->u.integer;
    }
  }

  json_value_free(value);
  return channels;
}

