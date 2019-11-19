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

#endif

