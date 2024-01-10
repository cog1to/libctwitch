#ifndef _H_TWITCH_HELIX_CHANNELS
#define _H_TWITCH_HELIX_CHANNELS

#include <stdlib.h>
#include <ctwitch/helix/data.h>

/**
 * Download one page of followers data for given channel and returns an array of
 * twitch_helix_follower structs with that data.
 *
 * @param client_id Twitch API client ID.
 * @param token User access token. Must be issued with
 * "moderator:read:followers" permission.
 * @param channel_id Channel ID.
 * @param first Page size. Must be between 1 and 100, including.
 * @param after Pagination cursor.
 * @param total Will return total number of followers for the channel.
 * @param next_cursor Will return next cursor value (basically a page pointer
 * instead of offset).
 *
 * @return Followers list. Deallocate with twitch_helix_follower_list_free().
 * */
twitch_helix_follower_list *twitch_helix_get_channel_followers(
	const char *client_id,
	const char *token,
	const char *channel_id,
	const char *user_id,
	int first,
	const char *after,
	int *total,
	char **next
);

/**
 * Downloads up to `limit` number of channel followers for given channel.
 *
 * @param client_id Twitch API client ID.
 * @param token User access token. Must be issued with
 * "moderator:read:followers" permission.
 * @param channel_id Channel ID.
 * @param user_id Used to check if a specific user follows given channel ID. If
 * you're interested in all followers, pass NULL. If not NULL, it will return
 * either a single entry list in case user followers the channel, or an empty
 * list.
 * @param limit Max number of followers to read. Pass 0 to get all followers.
 *
 * @return List of up to given amount of followers of a channel in a
 * twitch_helix_follower_list struct.
 * */
twitch_helix_follower_list *twitch_helix_get_all_channel_followers(
	const char *client_id,
	const char *token,
	const char *channel_id,
	const char *user_id,
	int limit
);

/**
 * Downloads the list of teams a channel belongs to.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @param channel_id Channel ID.
 *
 * @return Instance of twitch_team_list containing all channel's teams.
 * */
twitch_helix_team_list *twitch_helix_get_channel_teams(
	const char *client_id,
	const char *token,
	const char *channel_id
);

#endif

