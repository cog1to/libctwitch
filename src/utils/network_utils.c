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

CURLcode twitch_post(const char *url, string_t *output) {
  // Initialize curl.
  CURL *curl;
  curl = curl_easy_init();

  // Headers list.
  struct curl_slist *headers = NULL;

  // Basic headers.
  headers = curl_slist_append(headers, "Accept: application/json");

  // Set headers.
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  // Set URL.
  curl_easy_setopt(curl, CURLOPT_URL, url);

  // Setup output buffer.
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitch_writefunc);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

  // Perform curl operation.
  CURLcode code = curl_easy_perform(curl);

  // Cleanup.
  curl_easy_cleanup(curl);

  return code;
}

json_value *twitch_auth_post_json(const char *url) {
  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_post(url, output);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }

  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse JSON.\n");
  }

  return value;
}

