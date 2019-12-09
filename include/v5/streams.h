/**
 * Twitch V5 API - Streams
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_V5_STREAMS
#define _H_TWITCH_V5_STREAMS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "data.h"

/**
 * Returns one page of items describing live streams queried by channel list, game, language.
 *
 * @param client_id Twitch API client ID.
 * @param channel_ids_count Number of channel IDs to apply as filters. Only streams present in the list will be returned.
 * @param channel_ids Array of strings with channel IDs.
 * @param game Game name filter. Has to be an exact match with one of the games registered on Twitch.
 * @param stream_type Stream type. Can be "all", "live", or "playlist". If not specified, "all" will be assumed by the API.
 * @param language. Locale ID string specifying the language filter. Example: "en", "fi", "fr". If not specified,
 *      Twitch API will not use any language filters.
 * @param limit Page size.
 * @param offset List offset.
 * @param total Used to return a total number of items as reported by Twitch API.
 *
 * @return An instance of twitch_v5_stream_list struct containing stream data. Free it later with twitch_v5_stream_list_free().
 * */
twitch_v5_stream_list *twitch_v5_get_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int limit, int offset, int *total);

/**
 * Returns full list of items describing live streams queried by channel list, game, language; internally downloaded
 * by calling paged version of this function.
 *
 * @param client_id Twitch API client ID.
 * @param channel_ids_count Number of channel IDs to apply as filters. Only streams present in the list will be returned.
 * @param channel_ids Array of strings with channel IDs.
 * @param game Game name filter. Has to be an exact match with one of the games registered on Twitch.
 * @param stream_type Stream type. Can be "all", "live", or "playlist". If not specified, "all" will be assumed by the API.
 * @param language. Locale ID string specifying the language filter. Example: "en", "fi", "fr". If not specified, Twitch API will not use any language filters.
 *
 * @return An instance of twitch_v5_stream_list struct containing stream data. Free it later with twitch_v5_stream_list_free().
 */
twitch_v5_stream_list *twitch_v5_get_all_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language);

/**
 * Returns a stream object for given channel, if it's online. Otherwise returns NULL.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID to check.
 * @param stream_type Restricts the output to only return stream of a given type. Can be "all", "live", or "playlist". Defaults to "live".
 *
 * @return A new twitch_v5_stream struct describing the stream for specified channel, or NULL of channel is offline.
 */
twitch_v5_stream *twitch_v5_get_stream_by_user(const char *client_id, const char *channel_id, const char *stream_type);

/**
 * Returns stream summary overall or for a given game.
 *
 * @param client_id Twitch API client ID.
 * @param game Name of the game to get summary for. Can be NULL, in which case Twitch will return the overall stats for the whole site.
 *
 * @return Statistics for the site or specified game. If game is not found, will return NULL.
 */
twitch_v5_summary *twitch_v5_get_summary(const char *client_id, const char *game);

/**
 * Returns one page of featured streams. Each element in an array is a pointer to dynamically allocated twitch_v5_featured_stream struct.
 * Don't forget to release the allocated memory later by calling twitch_v5_feature_stream_list_free() function;
 *
 * @param client_id Twitch API client ID.
 * @param limit Page limit. Maximum allowed by the API: 100.
 * @param offset Page offset.
 * @param Will contain total number of items in the list.
 *
 * @return Instance of twitch_v5_featured_stream_list struct containing featured streams data.
 */
twitch_v5_featured_stream_list *twitch_v5_get_featured_streams(const char *client_id, int limit, int offset, int *total);

/**
 * Returns all featured streams. Each element in an array is a pointer to dynamically allocated twitch_v5_featured_stream struct.
 * Don't forget to release the allocated memory later by calling twitch_v5_featured_stream_list_free() function;
 *
 * @param client_id Twitch API client ID.
 *
 * @return Instance of twitch_v5_featured_stream_list struct containing featured streams data.
 */
twitch_v5_featured_stream_list *twitch_v5_get_all_featured_streams(const char *client_id);

#endif

