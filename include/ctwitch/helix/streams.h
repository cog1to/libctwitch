/**
 * Twitch Helix API - Streams
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_STREAMS
#define _H_TWITCH_HELIX_STREAMS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <ctwitch/common.h>
#include <ctwitch/helix/data.h>

/**
 * Returns one page of live streams data for given parameters.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param error Error holder struct.
 * @param game_id ID of the specific game to query.
 * @param language Language filter.
 * @param users_count Number of user IDs in the users list.
 * @param users ID of specific users to query.
 * @param logins_count Number of user names in the users list.
 * @param logins User names to query.
 * @param limit Page size.
 * @param after Page offset cursor.
 * @param total Returns a total number of items for given params.
 * @param next Returns cursor string to use in a request to fetch the next page
 * of data.
 *
 * @return List of live streams matching given parameters.
 */
twitch_helix_stream_list *twitch_helix_get_streams(
	const char *client_id,
	const char *auth,
	twitch_error *error,
	long long game_id,
	const char *language,
	int users_count,
	const char **users,
	int logins_count,
	const char **logins,
	int limit,
	const char *after,
	int *total,
	char **next
);

/**
 * Returns full list of live streams data for given parameters.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param error Error holder struct.
 * @param game_id ID of the specific game to query.
 * @param language Language filter.
 * @param users_count Number of user IDs in the users list.
 * @param users ID of specific users to query.
 * @param logins_count Number of user names in the users list.
 * @param logins User names to query.
 *
 * @return List of live streams matching given parameters.
 */
twitch_helix_stream_list *twitch_helix_get_all_streams(
	const char *client_id,
	const char *auth,
	twitch_error *error,
	const char *game_id,
	const char *language,
	int users_count,
	const char **users,
	int logins_count,
	const char **logins
);

#endif
