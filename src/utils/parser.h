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
 * Parses given json_value object of type 'json_array' using provided parser function.
 *
 * @param value The value holding the array to parse.
 * @param count Number of items in the array.
 * @param parser Parser function.
 *
 * @return Pointer to a newly allocated array of pointers to parsed objects. You will need to manually free
 *     the memory afterwards.
 */
void **parse_json_array(json_value *value, int *count, void *(*parser)(json_value *));

/**
 * Creates a new twitch_v5_user struct and tries to fill it with properties from provided JSON value.
 *
 * @param user_object JSON object holding user data fields.
 *
 * @return Pointer to newly allocated twitch_v5_user struct filled with data from JSON value.
 */
void *parse_user(json_value *user_object);

/**
 * Creates a new twitch_v5_channel struct and tries to fill it with properties from provided JSON value.
 *
 * @param channel_object JSON object holding user data fields.
 *
 * @return Pointer to newly allocated twitch_v5_channel struct filled with data from JSON value.
 */
void *parse_channel(json_value *channel_object);

/**
 * Creates a new twitch_v5_follow struct and tries to fill it with properties from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 *
 * @return Pointer to newly allocated twitch_v5_follow struct filled with data from JSON value.
 */
void *parse_follow(json_value *follow_object);

/**
 * Creates a new twitch_v5_stream struct and tries to fill it with properties from provided JSON value.
 *
 * @param stream_object JSON object holding user data fields.
 *
 * @return Pointer to newly allocated twitch_v5_stream struct filled with data from JSON value.
 */
void *parse_stream(json_value *stream_object);

/**
 * Creates a new twitch_v5_summary struct and tries to fill it with properties from provided JSON value.
 *
 * @param summary_object JSON object holding stream summary data fields.
 *
 * @return Pointer to newly allocated twitch_v5_summary struct filled with data from JSON value.
 */
void *parse_summary(json_value *summary_object);

/**
 * Creates a new twitch_v5_featured_stream struct and tries to fill it with properties from provided JSON value.
 *
 * @param featured_object JSON object holding featured stream data fields.
 *
 * @return Pointer to newly allocated twitch_v5_featured_stream struct filled with data from JSON value.
 */
void *parse_featured_stream(json_value *featured_object);

/**
 * Creates a new twitch_v5_game struct and tries to fill it with properties from provided JSON value.
 *
 * @param value JSON object holding game data fields.
 *
 * @return Pointer to newly allocated twitch_v5_game struct filled with data from JSON value.
 */
void *parse_game(json_value *value);

/**
 * Creates a new twitch_v5_top_game struct and tries to fill it with properties from provided JSON value.
 *
 * @param value JSON object holding top Game data fields.
 *
 * @return Pointer to newly allocated twitch_v5_top_game struct filled with data from JSON value.
 */
void *parse_top_game(json_value *value);

/**
 * Creates a new twitch_v5_follower struct and fills it with properties from JSON value.
 *
 * @param value JSON value holding follower data fields.
 *
 * @return Pointer to filled in twitch_v5_follower struct.
 */
void *parse_follower(json_value *value);

/**
 * Creates a new twitch_v5_team struct and fills it with properties from JSON value.
 *
 * @param value JSON value holding data fields.
 *
 * @return Pointer to filled in twitch_v5_team struct.
 */
void *parse_team(json_value *value);

/**
 * Creates a new twitch_v5_community struct and fills it with properties from JSON value.
 *
 * @param value JSON value holding data fields.
 *
 * @return Pointer to filled in twitch_v5_community struct.
 */
void *parse_community(json_value *value);

/**
 * Creates a new twitch_v5_resolutions struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_resolutions struct with data from JSON.
 */
void *parse_resolutions(json_value *value);

/**
 * Creates a new twitch_v5_fps struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_fps struct with data from JSON.
 */
void *parse_fps(json_value *value);

/**
 * Creates a new twitch_v5_channel_ref struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_channel_ref struct with data from JSON.
 */
void *parse_channel_ref(json_value *value);

/**
 * Creates a new twitch_v5_thumbnail struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_thumbnail struct with data from JSON.
 */
void *parse_thumbnail(json_value *value);

/**
 * Creates a new twitch_v5_thumbnail_list struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_thumbnail_list struct with data from JSON.
 */
void *parse_thumbnail_list(json_value *value);

/**
 * Creates a new twitch_v5_video_thumbnails struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_video_thumbnails struct with data from JSON.
 */
void *parse_video_thumbnails(json_value *value);

/**
 * Creates a new twitch_v5_video struct and fills it with properties from JSON value.
 *
 * @param value JSON value to parse.
 *
 * @return pointer to twitch_v5_video struct with data from JSON.
 */
void *parse_video(json_value *value);

/**
 * Creates a new twitch_helix_auth_token struct and fills it with properties from JSON value.
 *
 * @param value: JSON value to parse.
 *
 * @return Pointer to twith_helix_auth_token struct with data from JSON.
 */
void *parse_auth_token(json_value *value);

/**
 * Creates a new twitch_helix_user struct and tries to fill it with properties from provided JSON value.
 *
 * @param user_object JSON object holding user data fields.
 *
 * @return Pointer to newly allocated twitch_helix_user struct filled with data from JSON value.
 */
void *parse_helix_user(json_value *user_object);

/**
 * Creates a new twitch_helix_follow struct and tries to fill it with properties from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 *
 * @return Pointer to newly allocated twitch_helix_follow struct filled with data from JSON value.
 */
void *parse_helix_follow(json_value *follow_object);

/**
 * Creates a new twitch_helix_channel_follow struct and tries to fill it with properties from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 *
 * @return Pointer to newly allocated twitch_helix_channel_follow struct filled with data from JSON value.
 */
void *parse_helix_channel_follow(json_value *follow_object);

/**
 * Creates a new twitch_helix_stream struct and tries to fill it with properties from provided JSON value.
 *
 * @param stream_object JSON object holding stream data fields.
 *
 * @return Pointer to newly allocated twitch_helix_stream struct filled with data from JSON value.
 */
void *parse_helix_stream(json_value *stream_object);

#endif

