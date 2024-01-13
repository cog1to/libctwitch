/**
 * Twitch API: Common data types.
 *
 * @author Alexander Rogachev
 * @version 0.1
 *
 */
#ifndef _H_TWITCH_COMMON
#define _H_TWITCH_COMMON

/** String list **/

typedef struct {
	int count;
	char **items;
} twitch_string_list;

/**
 * Allocates a dynamic string list struct.
 *
 * @return A pointer to the allocated twitch_string_list struct.
 */
twitch_string_list *twitch_string_list_alloc();

/**
 * Releases memory occupied by the string list and all if its items.
 *
 * @param list List to deallocate
 */
void twitch_string_list_free(twitch_string_list *list);

/** Errors **/

typedef struct {
	long curl_code; // curl result code.
	long http_code; // HTTP response code.
	char *output;   // Response body, if any was returned. Currently not used.
} twitch_error;

#endif

