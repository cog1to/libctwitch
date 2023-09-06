/**
 * Twitch Helix API - Users
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_USERS
#define _H_TWITCH_HELIX_USERS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "data.h"

/**
 * Returns a pointer to twitch_helix_user structure describing user found by given user ID.
 *
 * @param login User login name.
 * @param client_id Twitch API client ID.
 * @param auth Authorization token.
 *
 * @return Dynamically allocated twitch_helix_user struct describing user.
 * You have to manually free the memory using twitch_helix_user_free() function.
 */
twitch_helix_user *twitch_helix_get_user(const char *client_id, const char *auth, const char *login);

/**
 * Returns a list of user data for given logins.
 *
 * @param login_count Number of login names.
 * @param login_names Login names array.
 * @param client_id Twitch API client ID.
 * @param auth Authorization token.
 *
 * @return Dynamically allocated twitch_helix_user_list struct describing user.
 * You have to manually free the memory using twitch_helix_user_list_free() function.
 */
twitch_helix_user_list *twitch_helix_get_users(
  const char *client_id,
  const char *auth,
  int logins_count,
  const char **logins
);

/**
 * Returns one page of follow data for given user(s). if `from_id` is specified,
 * returns users that given user follows. If `to_id` is specified, returns users who
 * follow given user. If both are specified, returns a list with a single follow entity
 * if first user follows the second one, or an empty list if they're not.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param from_id ID of a user to query for outgoing follows.
 * @param to_id ID of a user to query for incoming follows.
 * @param limit Page limit.
 * @param after Page offset cursor.
 * @param total Returns a total number of follows for given params.
 * @param next Returns cursor string to use in a request to fetch the next page of data.
 *
 * @return List of follows matching given parameters.
 */
twitch_helix_follow_list *twitch_helix_get_follows(
  const char *client_id,
  const char *auth,
  long long from_id,
  long long to_id,
  int limit,
  const char *after,
  int *total,
  char *next
);

/**
 * Returns list of follow data for given user(s). if `from_id` is specified,
 * returns users that given user follows. If `to_id` is specified, returns users who
 * follow given user. If both are specified, returns a list with a single follow entity
 * if first user follows the second one, or an empty list if they're not.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param from_id ID of a user to query for outgoing follows.
 * @param to_id ID of a user to query for incoming follows.
 *
 * @return List of follows matching given parameters.
 */
twitch_helix_follow_list *twitch_helix_get_all_follows(
  const char *client_id,
  const char *auth,
  long long from_id,
  long long to_id
);

/**
 * Returns one page of follow data for given channel. If `broadcaster_id` is
 * specified, the result contains a list with one item if user follows given
 * broadcaster, or an empty list otherwise.
 *
 * Requires a valid user access token obtained with "user:read:follows" scope
 * instead of standard app access token.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param user_id ID of a user to query for outgoing follows.
 * @param broadcaster_id ID of a user to query for incoming follows.
 * @param limit Page limit.
 * @param after Page offset cursor.
 * @param total Returns a total number of follows for given params.
 * @param next Returns cursor string to use in a request to fetch the next page
 * of data.
 *
 * @return List of follows matching given parameters.
 */
twitch_helix_channel_follow_list *twitch_helix_get_channel_follows(
  const char *client_id,
  const char *auth,
  long long user_id,
  long long broadcaster_id,
  int limit,
  const char *after,
  int *total,
  char *next
);

/**
 * Returns list of follow data for given channel. If `broadcaster_id` is
 * specified, the result contains a list with one item if user follows given
 * broadcaster, or an empty list otherwise.
 *
 * Requires a valid user access token obtained with "user:read:follows" scope
 * instead of standard app access token.
 *
 * @param client_id Twitch Client ID.
 * @param auth Authorization token.
 * @param user_id ID of a user to query for outgoing follows.
 * @param broadcaster_id ID of a user to query for incoming follows.
 *
 * @return List of follows matching given parameters.
 */
twitch_helix_channel_follow_list *twitch_helix_get_all_channel_follows(
  const char *client_id,
  const char *auth,
  long long user_id,
  long long broadcaster_id
);

#endif
