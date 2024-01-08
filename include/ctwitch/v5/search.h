/**
 * Twitch V5 API - Search
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_V5_SEARCH
#define _H_TWITCH_V5_SEARCH

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctwitch/v5/data.h>

/**
 * Performs channel search for given query string.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehabd, because the
 * method performs its own URL-escaping.
 * @param limit Page size.
 * @param offset Search results list offset.
 * @param total Will hold the total number of found channels.
 *
 * @return Instance of twitch_channel_list containing channels matching given
 * search string. Don't forget to free the memory allocated for the list.
 */
twitch_v5_channel_list *twitch_v5_search_channels(
	const char *client_id,
	const char *query,
	int limit,
	int offset,
	int *total
);

/**
 * Performs channel search for given query string and downloads all pages of
 * results.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehabd, because the
 * method performs its own URL-escaping.
 * @param size Will hold the number of downloaded channel objects.
 *
 * @return Instance of twitch_channel_list containing channels matching given
 * search string. Don't forget to free the memory allocated for the list.
 */
twitch_v5_channel_list *twitch_v5_search_all_channels(
	const char *client_id,
	const char *query
);

/**
 * Performs stream search with given query string and stream type filter and
 * return one page of the results.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehand, because the
 * method performs URL-escaping internally.
 * @param filter Stream type filter. If set to anything other than 'none', only
 * streams of the given type will be returned.
 * @param limit Page size.
 * @param offset List offset.
 * @param total Will hold the total number of found streams.
 *
 * @return Instance of twitch_stream_list struct with streams matching given
 * search string. Don't forget to free the memory with
 * twitch_v5_stream_list_free().
 */
twitch_v5_stream_list *twitch_v5_search_streams(
	const char *client_id,
	const char *query,
	stream_filter filter,
	int limit,
	int offset,
	int *total
);

/**
 * Performs stream search for given query string and filter, and downloads all
 * pages of results.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehand, because the
 * method performs its own URL-escaping.
 * @param filter Stream type filter. If set to anything other than 'none', only
 * streams of the given type will be returned.
 *
 * @return Instance of twitch_stream_list struct with streams matching given
 * search string. Don't forget to free the memory with
 * twitch_v5_stream_list_free().
 */
twitch_v5_stream_list *twitch_v5_search_all_streams(
	const char *client_id,
	const char *query,
	stream_filter filter
);

/**
 * Performs game search with given query string and live filter.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehand, because the
 * method performs URL-escaping internally.
 * @param live Live filter. If set to 'true', will return only games that have
 * at least one live stream.
 *
 * @return Array of twitch_game structs matching given search string. Don't
 * forget to free the memory allocated for each item and array itself.
 */
twitch_v5_game_list *twitch_v5_search_games(
	const char *client_id,
	const char *query,
	bool live
);

#endif

