/**
 * Network-related utility functions.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_NETWORK_UTILS
#define _H_NETWORK_UTILS

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
typedef string_t *(*page_url_builder)(void *, int limit, int offset);

/**
 * Convenience JSON parser function type.
 */
typedef void *(*parser_func)(json_value *);

/**
 * Helper function to append paging params to the end of the URL query
 *
 * @param url URL string to add limit and offset params to.
 * @param limit Page size.
 * @param offset Page offset.
 * @parm is_first_param Flag indicating whether there are query parameters already added
 *   to the URL.
 */
void append_paging_params(string_t *url, int limit, int offset, bool is_first_param);

/**
 * Returns URL-encoded version of the string.
 *
 * @param string Input string to URL-encode.
 *
 * @return URL-encoded string.
 */
const char *url_encode(const char *string);

/**
 * Performs GET request to Twitch API and writes the output to dynamic string.
 *
 * @param client_id Twitch API client ID string.
 * @param url Target URL string.
 * @param output String to write output to.
 *
 * @return cURL request result code.
 */
CURLcode twitch_v5_get(const char *client_id, const char *url, string_t *output);

/**
 * Performs a GET request to given Twitch API endpoint URL, and returns parsed JSON value.
 *
 * @param client_id Twitch API client ID.
 * @param url Target API endpoint URL.
 *
 * @return Parsed JSON value. (see utils/json library).
 */
json_value *twitch_v5_get_json(const char *client_id, const char *url);

/**
 * Downloads one page of paged data from Twitch API and parses it with given parsing params.
 * Paged data endpoints usually return data as a JSON object with the following structure:
 *
 * {
 *   "_total": 123,
 *   "<entity_name>": [{...}, {...}, {...},...]
 * }
 *
 * There "_total" indicate the overall number of items matching given request or query, and
 * "<entity_name>" contain items for requested page. Entity name varies from endpoint to endpoint,
 * as well as format of each item.
 *
 * @param client_id Twitch API client ID.
 * @param builder Twitch API URL builder function.
 * @param params URL/request params to provide to the builder function.
 * @param limit Page size.
 * @param offset Page offset.
 * @param values_key JSON key that should contain the list values.
 * @param parser Parser function to parse each value object inside the values JSON array.
 * @param size Returns number of parsed items.
 * @param total Returns total number of available items.
 *
 * @return Array of pointers to downloaded and parsed items.
 */
void **get_page(const char *client_id, page_url_builder builder, void *params, int limit, int offset, const char *values_key, parser_func parser, int *size, int *total);

/**
 * Downloads all pages of data from Twitch API for given query/entity type.
 *
 * @param client_id Twitch API client ID.
 * @param builder Twitch API URL builder function.
 * @param params URL/request params to provide to the builder function.
 * @param values_key JSON key that should contain the list values.
 * @param parser Parser function to parse each value object inside the values JSON array.
 * @param ignore_totals Flag indicating whether method should use '_total' value to determine
 *   the overall number of items to download, or should just go from page to page until 0 items
 *   is returned. This is useful in some cases when response doesn't contain '_total' prop, or
 *   '_total' value is not accurate. (when you try to get list of users but one of the users is
 *   banned, for example)
 * @param size Returns number of downloaded items.
 *
 * @return Array of pointers to downloaded and parsed items.
 */
void **get_all_pages(const char *client_id, page_url_builder builder, void *params, const char *values_key, parser_func parser, bool ignore_totals, int *size);

#endif

