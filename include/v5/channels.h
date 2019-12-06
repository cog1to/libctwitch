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
 * @param total Will return total number of followers for the channel.
 * @param next_cursor Will return next cursor value (basically a page pointer instead of offset).
 *
 * @return Followers list. Deallocate with twitch_follower_list_free().
 * */
twitch_follower_list *twitch_v5_get_channel_followers(const char *client_id, const char *channel_id, int limit, const char *cursor, const char *direction, int *total, char **next_cursor);

/**
 * Downloads the full list of channel followers for given channel.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 * @param direction Sorting direction. Can be either 'asc' or 'desc'.
 *
 * @return List of all followers of a channel in a twitch_follower_list struct.
 * */
twitch_follower_list *twitch_v5_get_all_channel_followers(const char *client_id, const char *channel_id, const char *direction);

/**
 * Downloads the list of channel teams.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 *
 * @return Instance of twitch_team_list containing all channel's teams.
 * */
twitch_team_list *twitch_v5_get_channel_teams(const char *client_id, const char *channel_id);

/**
 * Downloads the list of channel's communities.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID.
 *
 * @return List of communities in a twitch_community_list struct. You will have to manually free the memory and the array itself.
 */
twitch_community_list *twitch_v5_get_channel_communities(const char *client_id, const char *channel_id);

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
 * @param total Will return a total count of videos of the specific channel.
 *
 * @return List of videos in a twitch_video_list struct.
 * */
twitch_video_list *twitch_v5_get_channel_videos(const char *client_id, const char *channel_id, int limit, int offset, const char *broadcast_type, const char *language, char *sort, int* total);

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
 * @return List of all videos on a given channel inside twitch_video_list struct. 
 */
twitch_video_list *twitch_v5_get_all_channel_videos(const char *client_id, const char *channel_id, const char *broadcast_type, const char *language, char *sort);

#endif

