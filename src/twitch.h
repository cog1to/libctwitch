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
 * Returns a pointer to twitch_user structure describing user found by given username.
 *
 * @param username Username to lookup.
 * @param client_id Twitch API client ID.
 * @return Dynamically allocated twitch_user struct describing user. You have to manually free 
 *      the memory using twitch_user_free() function.
 */
twitch_user *twitch_v5_get_user(const char* username, const char* client_id);

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

#endif

