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
 * Convenience JSON parser function type.
 */
typedef void *(*parser_func)(json_value *);

/**
 * Performs a POST request to given Twitch API endpoint URL, and returns parsed
 * JSON value.
 *
 * @param url Target API endpoint URL.
 *
 * @return Parsed JSON value. (see utils/json library).
 */
json_value *twitch_auth_post_json(const char *url);

#endif

