/**
 * Twitch V5 API.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH
#define _H_TWITCH

#include <stdio.h>
#include <stdint.h>

#include "data.h"

/**
 * Returns a pointer to twitch_user structure describing user found by given user ID.
 *
 * @param id User ID.
 * @param client_id Twitch API client ID.
 * @return Dynamically allocated twitch_user struct describing user. You have to manually free
 *      the memory using twitch_user_free() function.
 */
twitch_user *twitch_v5_get_user(const char *client_id, const char *id);

/**
 * Returns a pointer to twitch_user structure describing user found by given username.
 *
 * @param username Username to lookup.
 * @param client_id Twitch API client ID.
 * @return Dynamically allocated twitch_user struct describing user. You have to manually free
 *      the memory using twitch_user_free() function.
 */
twitch_user *twitch_v5_get_user_by_username(const char *client_id, const char *username);

/**
 * Returns a list of users matching given list of logins.
 *
 * @param client_id Twitch API client ID.
 * @param usernames_count Size of the list of usernames. Should not exceed 100, accodring to API docs.
 * @param usernames List of usernames/logins to check.
 * @param total Used to return total number of users parsed from the response.
 *
 * @return Array of pointers to twitch_user objects. Don't forget to free them with twitch_user_free() function.
 */
twitch_user **twitch_v5_get_users(const char *client_id, int usernames_count, const char **usernames, int *total);

/**
 * Checks if specific user follows specific channel.
 *
 * @param client_id Twitch API client ID.
 * @param user_id User ID to check.
 * @param channel_id Channel ID to check for user's follow.
 *
 * @return Follow data if user does follow the channel, or NULL otherwise. Don't forget to free the twitch_follow struct
 *      with twitch_follow_free() function.
 */
twitch_follow *twitch_v5_check_user_follow(const char *client_id, const char *user_id, const char *channel_id);

/**
 * Returns one page of list of follows for a given user.
 *
 * @param client_id Twitch API client ID.
 * @param user_id Twitch User ID: a string representation of user's ID number.
 * @param direction List sort direction. Can be either "asc" or "desc".
 * @param sortby Sort property. Can be either "created_at", "last_broadcast", or "login".
 * @param limit Page size.
 * @param offset List offset.
 * @param size Used to return the number of downloaded items. Can be less than page size if there aren't enough items.
 * @param total Used to return the total number of items in the list reported by Twitch API.
 * @return Array of pointers to twitch_follow structs describing each user's follow. You have to manually free
 *      each item in the array, as well as array itself.
 */
twitch_follow **twitch_v5_get_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int limit, int offset, int *size, int *total);

/**
 * Returns list of all follows for a given user downloaded by internally calling paged version
 * until there are no more items. Currently uses Page size of 20.
 *
 * @param client_id Twitch API client ID.
 * @param user_id Twitch User ID: a string representation of user's ID number.
 * @param direction List sort direction. Can be either "asc" or "desc". If not specified, "desc" will be assumed by the API.
 * @param sortby Sort property. Can be either "created_at", "last_broadcast", or "login". If not specified, "created_at" will
 *      be used by the API as default.
 * @param size Used to return the number of downloaded items. Can be less than page size if there aren't enough items.
 *
 * @return Array of pointers to twitch_follow structs describing each user's follow. You have to manually free
 *      each item in the array, as well as array itself.
 */
twitch_follow **twitch_v5_get_all_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int *size);

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
 * @param size Used to return a number of downloaded items.
 * @param total Used to return a total number of items as reported by Twitch API.
 *
 * @return Array of pointers to twitch_stream structs describing each live stream. You have to manually free
 *      each item in the array, as well as array itself.
 */
twitch_stream **twitch_v5_get_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int limit, int offset, int *size, int *total);

/**
 * Returns full list of items describing live streams queried by channel list, game, language; internally downloaded
 * by calling paged version of this function.
 *
 * @param client_id Twitch API client ID.
 * @param channel_ids_count Number of channel IDs to apply as filters. Only streams present in the list will be returned.
 * @param channel_ids Array of strings with channel IDs.
 * @param game Game name filter. Has to be an exact match with one of the games registered on Twitch.
 * @param stream_type Stream type. Can be "all", "live", or "playlist". If not specified, "all" will be assumed by the API.
 * @param language. Locale ID string specifying the language filter. Example: "en", "fi", "fr". If not specified,
 *      Twitch API will not use any language filters.
 * @param size Used to return a number of downloaded items.
 *
 * @return Array of pointers to twitch_stream structs describing each live stream. You have to manually free
 *      each item in the array, as well as array itself.
 */
twitch_stream **twitch_v5_get_all_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int *size);

/**
 * Returns a stream object for given channel, if it's online. Otherwise returns NULL.
 *
 * @param client_id Twitch API client ID.
 * @param channel_id Channel ID to check.
 * @param stream_type Restricts the output to only return stream of a given type. Can be "all", "live", or "playlist". Defaults to "live".
 *
 * @return A new twitch_stream struct describing the stream for specified channel, or NULL of channel is offline.
 */
twitch_stream *twitch_v5_get_stream_by_user(const char *client_id, const char *channel_id, const char *stream_type);

/**
 * Returns stream summary overall or for a given game.
 *
 * @param client_id Twitch API client ID.
 * @param game Name of the game to get summary for. Can be NULL, in which case Twitch will return the overall stats for the whole site.
 *
 * @return Statistics for the site or specified game. If game is not found, will return NULL.
 */
twitch_summary *twitch_v5_get_summary(const char *client_id, const char *game);

/**
 * Returns one page of featured streams. Each element in an array is a pointer to dynamically allocated twitch_featured_stream struct.
 * Don't forget to release the allocated memory later by calling twitch_feature_stream_list_free() function;
 *
 * @param client_id Twitch API client ID.
 * @param limit Page limit. Maximum allowed by the API: 100.
 * @param offset Page offset.
 * @param size Number of downloaded items.
 *
 * @return Array of twitch_featured_stream pointers containing data for given page of featured streams.
 */
twitch_featured_stream **twitch_v5_get_featured_streams(const char *client_id, int limit, int offset, int *size);

/**
 * Returns all featured streams. Each element in an array is a pointer to dynamically allocated twitch_featured_stream struct.
 * Don't forget to release the allocated memory later by calling twitch_feature_stream_list_free() function;
 *
 * @param client_id Twitch API client ID.
 * @param size Number of downloaded items.
 *
 * @return Array of twitch_featured_stream pointers containing data for given page of featured streams.
 */
twitch_featured_stream **twitch_v5_get_all_featured_streams(const char *client_id, int *size);

/**
 * Performs channel search for given query string.
 *
 * @param client_id Twitch API client ID.
 * @param query Query string. Don't URL-escape this beforehabd, because the method performs its own URL-escaping.
 * @param limit Page size.
 * @param offset Search results list offset.
 * @param size Will hold the total number of downloaded channel objects.
 * @param total Will hold the total number of found channels.
 *
 * @return Array of twitch_channel structs matching given search string. Don't forget to free the memory allocated for each item and
 *     array itself.
 */
twitch_channel **twitch_v5_search_channels(const char *client_id, const char *query, int limit, int offset, int *size, int *total);

#endif

