/**
 * Network-related utility functions for Helix API.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_NETWORK_HELIX_UTILS
#define _H_NETWORK_HELIX_UTILS

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "string_utils.h"
#include "json/json.h"

/**
 * Convenience function type for URL builder functions.
 */
typedef string_t *(*helix_page_url_builder)(void *, int limit, const char *after);

/**
 * Convenience JSON parser function type.
 */
typedef void *(*parser_func)(json_value *);

/**
 * Helper function to append paging params to the end of the URL query
 *
 * @param url URL string to add limit and offset params to.
 * @param limit Page size.
 * @param after Offset cursor.
 * @parm is_first_param Flag indicating whether there are query parameters already added
 *   to the URL.
 */
void helix_append_cursor_params(string_t *url, int limit, const char *after, bool is_first_param);


/**
 * Performs GET request to Twitch Helix API and writes the output to dynamic string.
 *
 * @param client_id Twitch API client ID string.
 * @param auth Authorization token.
 * @param url Target URL string.
 * @param output String to write output to.
 *
 * @return cURL request result code.
 */
CURLcode twitch_helix_get(const char *client_id, const char *auth, const char *url, string_t *output);

/**
 * Performs a GET request to given Twitch API endpoint URL, and returns parsed JSON value.
 *
 * @param client_id Twitch API client ID.
 * @param auth Authorization token.
 * @param url Target API endpoint URL.
 *
 * @return Parsed JSON value. (see utils/json library).
 */
json_value *twitch_helix_get_json(const char *client_id, const char *auth, const char *url);

/**
 * Performs a POST request to given Twitch API endpoint URL, and returns parsed JSON value.
 *
 * @param url Target API endpoint URL.
 *
 * @return Parsed JSON value. (see utils/json library).
 */
json_value *twitch_auth_post_json(const char *url);

/**
 * Downloads one page of paged data from Twitch Helix API and parses it with given parsing params.
 * Paged data endpoints usually return data as a JSON object with the following structure:
 *
 * {
 *   "data": [{...}, {...}, {...},...],
 *   "pagination": {
 *     "cursor": "..."
 *   }
 * }
 *
 * There "cursor" contains cursor string to use as "after" parameter to fetch the next page of
 * data from the same response indicate the overall number of items matching given request or que
 *
 * @param client_id Twitch API client ID.
 * @param auth Authorization token.
 * @param builder Twitch API URL builder function.
 * @param params URL/request params to provide to the builder function.
 * @param limit Page size.
 * @param after Page offset.
 * @param parser Parser function to parse each value object inside the values JSON array.
 * @param size Returns number of parsed items.
 * @param next Returns cursor string to fetch the next page.
 *
 * @return Array of pointers to downloaded and parsed items.
 */
void **helix_get_page(
  const char *client_id,
  const char *auth,
  helix_page_url_builder builder,
  void *params,
  int limit,
  const char *after,
  parser_func parser,
  int *size,
  char *next
);

#endif

