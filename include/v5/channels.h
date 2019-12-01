#ifndef _H_TWITCH_V5_CHANNELS
#define _H_TWITCH_V5_CHANNELS

#include <stdlib.h>
#include "data.h"

/**
 * Download one page of followers data for given channel and returns an array of twitch_follower structs with that data.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 * @param limit Page size.
 * @param cursor "Tells the server where to start fetching the next set of results." I am not sure about the purpose of this param when you have "offset".
 * @param direction Sorting direction. Can be either 'asc' or 'desc'.
 * @param size Will return the number of parsed items in the array.
 * @param total Will return total number of followers for the channel.
 * @param next_cursor Will return next cursor value (basically a page pointer instead of offset).
 *
 * @return An array of pointers to dynamically allocated twitch_follower structs holding the downloaded data. You will have to manually free the memory and array itself later.
 */
twitch_follower **twitch_v5_get_channel_followers(const char *client_id, const char *channel_id, int limit, const char *cursor, const char *direction, int *size, int *total, char **next_cursor);

/**
 * Downloads the full list of channel followers for given channel.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 * @param direction Sorting direction. Can be either 'asc' or 'desc'.
 * @param size Will return the number of items in the array.
 *
 * @return An array of pointers to dynamically allocated twitch_follower structs holding the downloaded data. You will have to manually free the memory and array itself later.
 */
twitch_follower **twitch_v5_get_all_channel_followers(const char *client_id, const char *channel_id, const char *direction, int *size);

/**
 * Downloads the list of channel teams.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 *
 * @return An array of pointers to twitch_team structs describing which teams the channel is part of. You will have to manually free the memory and the array itself.
 */
twitch_team **twitch_v5_get_channel_teams(const char *client_id, const char *channel_id, int* size);

/**
 * Downloads the list of channel's communities.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 *
 * @return An array of pointers to twitch_community structs describing which communities the channel is part of. You will have to manually free the memory and the array itself.
 */
twitch_community **twitch_v5_get_channel_communities(const char *client_id, const char *channel_id, int* size);

/**
 * Downloads one page of channels' videos list.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 * @param limit Page size.
 * @param offset Page offset.
 * @param broadcast_type Comma-separated list of broadcast types. Possible values: 'archive', 'highlight', 'upload'. Default: all of them.
 * @param language Comma-separated list of language/locale abbreviations, e.g. 'en,es'. Default: all languages.
 * @param sort Sort field. Valid values: 'views', 'time'. Default: 'time'.
 * @param size Will return a number of downloaded and parsed items.
 * @param total Will return a total count of videos of the specific channel.
 *
 * @return Array of pointers to twitch_video structs with video data. You have to deallocate it later manually.
 */
twitch_video **twitch_v5_get_channel_videos(const char *client_id, const char *channel_id, int limit, int offset, const char *broadcast_type, const char *language, char *sort, int *size, int* total);

/**
 * Downloads all of channels' videos list.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 * @param broadcast_type Comma-separated list of broadcast types. Possible values: 'archive', 'highlight', 'upload'. Default: all of them.
 * @param language Comma-separated list of language/locale abbreviations, e.g. 'en,es'. Default: all languages.
 * @param sort Sort field. Valid values: 'views', 'time'. Default: 'time'.
 * @param size Will return a number of downloaded and parsed items.
 *
 * @return Array of pointers to twitch_video structs with video data. You have to deallocate it later manually.
 */
twitch_video **twitch_v5_get_all_channel_videos(const char *client_id, const char *channel_id, const char *broadcast_type, const char *language, char *sort, int *size);

#endif

