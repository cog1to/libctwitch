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
 * @return Dynamically allocated twitch_helix_user struct describing user. You have to manually free
 *      the memory using twitch_helix_user_free() function.
 */
twitch_helix_user *twitch_helix_get_user(const char *client_id, const char *auth, const char *login);

/**
 * Returns a list of user data for given logins.
 *
 * @param login_count Number of login names.
 * @param login_names Login names array.
 * @param client_id Twitch API client ID.
 * @param auth Authorization token.
 * @return Dynamically allocated twitch_helix_user_list struct describing user. You have to manually free
 *      the memory using twitch_helix_user_list_free() function.
 */
twitch_helix_user_list *twitch_helix_get_users(const char *client_id, const char *auth, int logins_count, const char **logins);

#endif
