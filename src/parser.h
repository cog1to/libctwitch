#ifndef _PARSER_H
#define _PARSER_H

#include "json.h"

void **parse_json_array(json_value *value, int *count, void *(*parser)(json_value *)); 

void *parse_user(json_value *user_object); 

void *parse_channel(json_value *channel_object); 

void *parse_follow(json_value *follow_object); 

void *parse_stream(json_value *stream_object);

#endif

