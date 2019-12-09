/**
 * Twitch V5 API - Users
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_V5_USERS
#define _H_TWITCH_V5_USERS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "data.h"

/**
 * Returns a pointer to twitch_v5_user structure describing user found by given user ID.
 *
 * @param id User ID.
 * @param client_id Twitch API client ID.
 * @return Dynamically allocated twitch_v5_user struct describing user. You have to manually free
 *      the memory using twitch_v5_user_free() function.
 */
twitch_v5_user *twitch_v5_get_user(const char *client_id, const char *id);

/**
 * Returns a pointer to twitch_v5_user structure describing user found by given username.
 *
 * @param username Username to lookup.
 * @param client_id Twitch API client ID.
 * @return Dynamically allocated twitch_v5_user struct describing user. You have to manually free
 *      the memory using twitch_v5_user_free() function.
 */
twitch_v5_user *twitch_v5_get_user_by_username(const char *client_id, const char *username);

/**
 * Returns a list of users matching given list of logins.
 *
 * @param client_id Twitch API client ID.
 * @param usernames_count Size of the list of usernames. Should not exceed 100, accodring to API docs.
 * @param usernames List of usernames/logins to check.
 *
 * @return List of users. */
twitch_v5_user_list *twitch_v5_get_users(const char *client_id, int usernames_count, const char **usernames);

/**
 * Checks if specific user follows specific channel.
 *
 * @param client_id Twitch API client ID.
 * @param user_id User ID to check.
 * @param channel_id Channel ID to check for user's follow.
 *
 * @return Follow data if user does follow the channel, or NULL otherwise. Don't forget to free the twitch_v5_follow struct
 *      with twitch_v5_follow_free() function.
 */
twitch_v5_follow *twitch_v5_check_user_follow(const char *client_id, const char *user_id, const char *channel_id);

/**
 * Returns one page of list of follows for a given user.
 *
 * @param client_id Twitch API client ID.
 * @param user_id Twitch User ID: a string representation of user's ID number.
 * @param direction List sort direction. Can be either "asc" or "desc".
 * @param sortby Sort property. Can be either "created_at", "last_broadcast", or "login".
 * @param limit Page size.
 * @param offset List offset.
 * @param total Used to return the total number of items in the list reported by Twitch API.
 *
 * @return List of follows. Deallocate with twitch_v5_follow_list_free(). */
twitch_v5_follow_list *twitch_v5_get_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int limit, int offset, int *total);

/**
 * Returns list of all follows for a given user downloaded by internally calling paged version
 * until there are no more items. Currently uses Page size of 20.
 *
 * @param client_id Twitch API client ID.
 * @param user_id Twitch User ID: a string representation of user's ID number.
 * @param direction List sort direction. Can be either "asc" or "desc". If not specified, "desc" will be assumed by the API.
 * @param sortby Sort property. Can be either "created_at", "last_broadcast", or "login". If not specified, "created_at" will be used by the API as default.
 *
 * @return List of follows.
 */
twitch_v5_follow_list *twitch_v5_get_all_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby);

#endif

