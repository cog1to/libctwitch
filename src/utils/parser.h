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
 * Creates a new twitch_user struct and tries to fill it with properties from provided JSON value.
 *
 * @param user_object JSON object holding user data fields.
 * 
 * @return Pointer to newly allocated twitch_user struct filled with data from JSON value.
 */
void *parse_user(json_value *user_object);

/**
 * Creates a new twitch_channel struct and tries to fill it with properties from provided JSON value.
 *
 * @param channel_object JSON object holding user data fields.
 * 
 * @return Pointer to newly allocated twitch_channel struct filled with data from JSON value.
 */
void *parse_channel(json_value *channel_object);

/**
 * Creates a new twitch_follow struct and tries to fill it with properties from provided JSON value.
 *
 * @param follow_object JSON object holding follow data fields.
 * 
 * @return Pointer to newly allocated twitch_follow struct filled with data from JSON value.
 */
void *parse_follow(json_value *follow_object);

/**
 * Creates a new twitch_stream struct and tries to fill it with properties from provided JSON value.
 *
 * @param stream_object JSON object holding user data fields.
 * 
 * @return Pointer to newly allocated twitch_stream struct filled with data from JSON value.
 */
void *parse_stream(json_value *stream_object);

/**
 * Creates a new twitch_summary struct and tries to fill it with properties from provided JSON value.
 *
 * @param summary_object JSON object holding stream summary data fields.
 * 
 * @return Pointer to newly allocated twitch_summary struct filled with data from JSON value.
 */
void *parse_summary(json_value *summary_object);

/**
 * Creates a new twitch_featured_stream struct and tries to fill it with properties from provided JSON value.
 *
 * @param featured_object JSON object holding featured stream data fields.
 * 
 * @return Pointer to newly allocated twitch_featured_stream struct filled with data from JSON value.
 */
void *parse_featured_stream(json_value *featured_object);

/**
 * Creates a new twitch_game struct and tries to fill it with properties from provided JSON value.
 *
 * @param value JSON object holding game data fields.
 * 
 * @return Pointer to newly allocated twitch_game struct filled with data from JSON value.
 */
void *parse_game(json_value *value);

/**
 * Creates a new twitch_top_game struct and tries to fill it with properties from provided JSON value.
 *
 * @param value JSON object holding top Game data fields.
 * 
 * @return Pointer to newly allocated twitch_top_game struct filled with data from JSON value.
 */
void *parse_top_game(json_value *value);

#endif

