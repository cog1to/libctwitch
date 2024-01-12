/**
 * Twitch Helix API - Search
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_SEARCH
#define _H_TWITCH_HELIX_SEARCH

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctwitch/helix/data.h>

/**
 * Returns a page of games/categories matching given query.
 *
 * @param client_id Client ID.
 * @param token Access token.
 * @param query Query string.
 * @param first Page size. Must be between 1 and 100, inclusive.
 * @param after Page cursor.
 *
 * @return List of games/categories.
 */
twitch_helix_category_list *twitch_helix_get_categories(
	const char *client_id,
	const char *token,
	const char *query,
	int first,
	const char *after,
	char **next
);

/**
 * Returns a full list of games/categories matching given query.
 *
 * @param client_id Client ID.
 * @param token Access token.
 * @param query Query string.
 * @param limit Max number of games to return. Pass 0 to get all
 * games/categories.
 *
 * @return List of games/categories.
 */
twitch_helix_category_list *twitch_helix_get_all_categories(
	const char *client_id,
	const char *token,
	const char *query,
	int limit
);

/**
 * Returns a page of channels matching given query.
 *
 * @param client_id Client ID.
 * @param token Access token.
 * @param query Query string.
 * @param live_only Filter flag to return only live streams.
 * @param first Page size. Must be between 1 and 100, inclusive.
 * @param after Page cursor. If NULL, the method will return the first page.
 *
 * @return List of channels matching query and live filter value.
 */
twitch_helix_channel_search_item_list *twitch_helix_search_channels(
	const char *client_id,
	const char *token,
	const char *query,
	int live_only,
	int first,
	const char *after,
	char **next
);

/**
 * Returns a full list of channels matching given query.
 *
 * @param client_id Client ID.
 * @param token Access token.
 * @param query Query string.
 * @param live_only Filter flag to return only live streams.
 * @param limit Max number of streams to return. Pass 0 to get the full list.
 *
 * @return List of games/categories.
 */
twitch_helix_channel_search_item_list *twitch_helix_search_all_channels(
	const char *client_id,
	const char *token,
	const char *query,
	int live_only,
	int limit
);

#endif

