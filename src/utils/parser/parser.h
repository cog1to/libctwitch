/**
 * JSON parsers for specific data types.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _PARSER_H
#define _PARSER_H

#include "json/json.h"

/**
 * Parses given json_value object of type 'json_array' using provided parser
 * function.
 *
 * @param value The value holding the array to parse.
 * @param count Number of items in the array.
 * @param parser Parser function.
 *
 * @return Pointer to a newly allocated array of pointers to parsed objects.
 * You will need to manually free the memory afterwards.
 */
void **parse_json_array(
	json_value *value,
	int *count,
	void *(*parser)(json_value *)
);

/**
 * Creates a new twitch_app_access_token struct and fills it with properties
 * from JSON value.
 *
 * @param value: JSON value to parse.
 *
 * @return Pointer to twith_app_access_token struct with data from JSON.
 */
void *parse_auth_token(json_value *value);

/**
 * Creates a new twitch_user_access_token struct and fills it with properties
 * from JSON value.
 *
 * @param value: JSON value to parse.
 *
 * @return Pointer to twith_user_access_token struct with data from JSON.
 */
void *parse_user_auth_token(json_value *value);

/**
 * Creates a new twitch_helix_user struct and tries to fill it with properties
 * from provided JSON value.
 *
 * @param user_object JSON object holding user data fields.
 *
 * @return Pointer to newly allocated twitch_helix_user struct filled with data
 * from JSON value.
 */
void *parse_helix_user(json_value *user_object);

/**
 * Creates a new twitch_helix_follow struct and tries to fill it with properties
 * from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 *
 * @return Pointer to newly allocated twitch_helix_follow struct filled with
 * data from JSON value.
 */
void *parse_helix_follow(json_value *follow_object);

/**
 * Creates a new twitch_helix_channel_follow struct and tries to fill it with
 * properties from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 *
 * @return Pointer to newly allocated twitch_helix_channel_follow struct filled
 * with data from JSON value.
 */
void *parse_helix_channel_follow(json_value *follow_object);

/**
 * Creates a new twitch_helix_stream struct and tries to fill it with properties
 * from provided JSON value.
 *
 * @param stream_object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_stream struct filled with
 * data from JSON value.
 */
void *parse_helix_stream(json_value *stream_object);

/**
 * Creates a new twitch_helix_game struct and tries to fill it with properties
 * from provided JSON value.
 *
 * @param stream_object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_game struct filled with
 * data from JSON value.
 */
void *parse_helix_game(json_value *stream_object);

/**
 * Creates a new twitch_helix_team struct and tries to fill it with properties
 * from provided JSON value.
 *
 * @param stream_object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_team struct filled with
 * data from JSON value.
 */
void *parse_helix_team(json_value *team_object);

/**
 * Creates a new twitch_helix_follower struct and tries to fill it with
 * properties from provided JSON value.
 *
 * @param object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_follower struct filled with
 * data from JSON value.
 */
void *parse_helix_follower(json_value *object);

/**
 * Creates a new twitch_helix_segment struct and tries to fill it with
 * properties from provided JSON value.
 *
 * @param object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_segment struct filled with
 * data from JSON value.
 */
void *parse_helix_segment(json_value *object);

/**
 * Creates a new twitch_helix_video struct and tries to fill it with
 * properties from provided JSON value.
 *
 * @param object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_video struct filled with
 * data from JSON value.
 */
void *parse_helix_video(json_value *object);

/**
 * Creates a new twitch_helix_category struct and tries to fill it with
 * properties from provided JSON value.
 *
 * @param object JSON object holding category data fields.
 *
 * @return Pointer to newly allocated twitch_helix_category struct filled with
 * data from JSON value.
 */
void *parse_helix_category(json_value *object);

/**
 * Creates a new twitch_helix_channel_search_item struct and tries to fill it
 * with properties from provided JSON value.
 *
 * @param object JSON object holding category data fields.
 *
 * @return Pointer to newly allocated twitch_helix_channel_search_item struct
 * filled with data from JSON value.
 */
void *parse_helix_channel_search_item(json_value *object);

#endif

