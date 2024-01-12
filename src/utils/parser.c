#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <ctwitch/auth.h>
#include <ctwitch/helix/data.h>

#include "parser.h"
#include "string_utils.h"

/** JSON array parser **/

void **parse_json_array(
	json_value *value,
	int *count,
	void *(*parser)(json_value *)
) {
	if (value->type != json_array) {
		return NULL;
	}

	int length = value->u.array.length;
	if (length == 0) {
		*count = 0;
		return NULL;
	}

	void **array = malloc(length * sizeof(void*));

	for (int index = 0; index < length; index++) {
		json_value *element = value->u.array.values[index];
		void *parsed = (*parser)(element);
		array[index] = parsed;
	}

	*count = length;
	return array;
}

/** Generic entity parsing **/

void parse_id(void *dest, json_value *source) {
	if (source->type == json_integer) {
		*((int *)dest) = source->u.integer;
	} else if (source->type == json_string) {
		*((int *)dest) = strtoimax(source->u.string.ptr, NULL, 10);
	}
}

void parse_string(void *dest, json_value *source) {
	if (source->type == json_string) {
		*((char **)dest) = immutable_string_copy(source->u.string.ptr);
	}
}

void *_parse_string(json_value *source) {
	char *string = immutable_string_copy(source->u.string.ptr);
	return string;
}

void parse_string_list(void *dest, json_value *source) {
	if (source->type == json_array) {
		int size = 0;
		twitch_string_list *list = dest;

		char **items = (char **)parse_json_array(source, &size, &_parse_string);
		list->count = size;
		list->items = items;
	}
}

void make_string_list(void *dest, json_value *value) {
	if (value->type == json_array) {
		int size = 0;
		twitch_string_list *list = twitch_string_list_alloc();

		char **items = (char **)parse_json_array(value, &size, &_parse_string);
		list->count = size;
		list->items = items;

		*((void **)dest) = list;
	}
}

void parse_bool(void *dest, json_value *source) {
	*((bool *)dest) = source->u.boolean;
}

void parse_double(void *dest, json_value *source) {
	*((float *)dest) = source->u.dbl;
}

void parse_int(void *dest, json_value *source) {
	*((int *)dest) = source->u.integer;
}

typedef struct {
	char *name;
	void *dest;
	void(*parser)(void*, json_value*);
} field_spec;

void parse_entity(json_value *src, int count, field_spec *specs) {
	for (int prop_ind = 0; prop_ind < src->u.object.length; prop_ind++) {
		for (int spec_ind = 0; spec_ind < count; spec_ind++) {
			if (strcmp(src->u.object.values[prop_ind].name, specs[spec_ind].name) == 0) {
				(*(specs[spec_ind].parser))(
					specs[spec_ind].dest,
					src->u.object.values[prop_ind].value
				);
			}
		}
	}
}

/** User **/

void *parse_helix_user(json_value *user_object) {
	twitch_helix_user *user = twitch_helix_user_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &user->id,
			.parser = &parse_id
		},
		{
			.name = "display_name",
			.dest = &user->display_name,
			.parser = &parse_string
		},
		{
			.name = "login",
			.dest = &user->login,
			.parser = &parse_string
		},
		{
			.name = "type",
			.dest = &user->type,
			.parser = &parse_string
		},
		{
			.name = "broadcaster_type",
			.dest = &user->broadcaster_type,
			.parser = &parse_string
		},
		{
			.name = "description",
			.dest = &user->description,
			.parser = &parse_string
		},
		{
			.name = "profile_image_url",
			.dest = &user->profile_image_url,
			.parser = &parse_string
		},
		{
			.name = "created_at",
			.dest = &user->created_at,
			.parser = &parse_string
		},
		{
			.name = "offline_image_url",
			.dest = &user->offline_image_url,
			.parser = &parse_string
		},
		{
			.name = "view_count",
			.dest = &user->view_count,
			.parser = &parse_int
		}
	};
	parse_entity(user_object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)user;
}

/** Streams **/

void *parse_helix_stream(json_value *stream_object) {
	twitch_helix_stream *stream = twitch_helix_stream_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &stream->id,
			.parser = &parse_id
		},
		{
			.name = "user_id",
			.dest = &stream->user_id,
			.parser = &parse_id
		},
		{
			.name = "user_name",
			.dest = &stream->user_name,
			.parser = &parse_string
		},
		{
			.name = "game_id",
			.dest = &stream->game_id,
			.parser = &parse_id
		},
		{
			.name = "game_name",
			.dest = &stream->game_name,
			.parser = &parse_string
		},
		{
			.name = "type",
			.dest = &stream->type,
			.parser = &parse_string
		},
		{
			.name = "title",
			.dest = &stream->title,
			.parser = &parse_string
		},
		{
			.name = "viewer_count",
			.dest = &stream->viewer_count,
			.parser = &parse_int
		},
		{
			.name = "started_at",
			.dest = &stream->started_at,
			.parser = &parse_string
		},
		{
			.name = "language",
			.dest = &stream->language,
			.parser = &parse_string
		},
		{
			.name = "thumbnail_url",
			.dest = &stream->thumbnail_url,
			.parser = &parse_string
		}
	};
	parse_entity(stream_object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)stream;
}

/** Follow **/

void *parse_helix_channel_follow(json_value *follow_object) {
	twitch_helix_channel_follow *follow = twitch_helix_channel_follow_alloc();

	field_spec schema[] = {
		{
			.name = "broadcaster_id",
			.dest = &follow->broadcaster_id,
			.parser = &parse_id
		},
		{
			.name = "broadcaster_name",
			.dest = &follow->broadcaster_name,
			.parser = &parse_string
		},
		{
			.name = "broadcaster_login",
			.dest = &follow->broadcaster_login,
			.parser = &parse_string
		},
		{
			.name = "followed_at",
			.dest = &follow->followed_at,
			.parser = &parse_string
		}
	};
	parse_entity(follow_object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)follow;
}

/** Games **/

void *parse_helix_game(json_value *game_object) {
	twitch_helix_game *game = twitch_helix_game_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &game->id,
			.parser = &parse_string
		},
		{
			.name = "igdb_id",
			.dest = &game->igdb_id,
			.parser = &parse_string
		},
		{
			.name = "name",
			.dest = &game->name,
			.parser = &parse_string
		},
		{
			.name = "box_art_url",
			.dest = &game->box_art_url,
			.parser = &parse_string
		}
	};
	parse_entity(game_object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)game;
}

/** Auth **/

void *parse_auth_token(json_value *value) {
	twitch_app_access_token *token = twitch_app_access_token_alloc();

	field_spec schema[] = {
		{
			.name = "access_token",
			.dest = &token->token,
			.parser = &parse_string
		},
		{
			.name = "expires_in",
			.dest = &token->expires_in,
			.parser = &parse_int
		},
		{
			.name = "token_type",
			.dest = &token->token_type,
			.parser = &parse_string
		},
	};
	parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)token;
}

void *parse_user_auth_token(json_value *value) {
	twitch_user_access_token *token = twitch_user_access_token_alloc();

	field_spec schema[] = {
		{
			.name = "access_token",
			.dest = &token->access_token,
			.parser = &parse_string
		},
		{
			.name = "refresh_token",
			.dest = &token->refresh_token,
			.parser = &parse_string
		},
		{
			.name = "expires_in",
			.dest = &token->expires_in,
			.parser = &parse_int
		},
		{
			.name = "token_type",
			.dest = &token->token_type,
			.parser = &parse_string
		},
		{
			.name = "scope",
			.dest = &token->scopes,
			.parser = &parse_string_list
		},
	};
	parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)token;
}

/** Teams **/

void *parse_team_member(json_value *value) {
	twitch_helix_team_member *member = twitch_helix_team_member_alloc();

	field_spec schema[] = {
		{
			.name = "user_id",
			.dest = &member->id,
			.parser = &parse_string
		},
		{
			.name = "user_name",
			.dest = &member->name,
			.parser = &parse_string
		},
		{
			.name = "user_login",
			.dest = &member->login,
			.parser = &parse_string
		},
	};
	parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)member;
}

void _parse_team_member_list(void *dest, json_value *value) {
	if (value->type == json_array) {
		int size = 0;
		twitch_helix_team_member_list *list = twitch_helix_team_member_list_alloc();

		twitch_helix_team_member **items =
			(twitch_helix_team_member **)parse_json_array(
				value,
				&size,
				&parse_team_member
			);
		list->count = size;
		list->items = items;

		*((void **)dest) = list;
	}
}

void *parse_helix_team(json_value *value) {
	json_value *team_object = NULL;

	if (value->type == json_array) {
		int length = value->u.object.length;
		for (int x = 0; x < length; x++) {
			if (strcmp(value->u.object.values[x].name, "data") == 0) {
				json_value *elements_value = value->u.object.values[x].value;
				int elements_length = elements_value->u.array.length;
				if (elements_length == 0) {
					break;
				}

				team_object = elements_value->u.array.values[0];
			}
		}
	} else if (value->type == json_object) {
		team_object = value;
	} else {
		return NULL;
	}

	if (team_object == NULL) {
		return NULL;
	}

	twitch_helix_team *team = twitch_helix_team_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &team->id,
			.parser = &parse_string
		},
		{
			.name = "created_at",
			.dest = &team->created_at,
			.parser = &parse_string
		},
		{
			.name = "updated_at",
			.dest = &team->updated_at,
			.parser = &parse_string
		},
		{
			.name = "background_image_url",
			.dest = &team->background,
			.parser = &parse_string
		},
		{
			.name = "thumbnail_url",
			.dest = &team->thumbnail,
			.parser = &parse_string
		},
		{
			.name = "banner",
			.dest = &team->banner,
			.parser = &parse_string
		},
		{
			.name = "info",
			.dest = &team->info,
			.parser = &parse_string
		},
		{
			.name = "team_display_name",
			.dest = &team->display_name,
			.parser = &parse_string
		},
		{
			.name = "team_name",
			.dest = &team->name,
			.parser = &parse_string
		},
		{
			.name = "users",
			.dest = &team->users,
			.parser = &_parse_team_member_list
		},
	};
	parse_entity(team_object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)team;
}

/** Followers **/

void *parse_helix_follower(json_value *object) {
	twitch_helix_follower *follower = twitch_helix_follower_alloc();

	field_spec schema[] = {
		{
			.name = "user_id",
			.dest = &follower->user_id,
			.parser = &parse_string
		},
		{
			.name = "user_name",
			.dest = &follower->user_name,
			.parser = &parse_string
		},
		{
			.name = "user_login",
			.dest = &follower->user_login,
			.parser = &parse_string
		},
		{
			.name = "followed_at",
			.dest = &follower->followed_at,
			.parser = &parse_string
		}
	};
	parse_entity(object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)follower;
}

/** Videos **/

void *parse_helix_segment(json_value *object) {
	twitch_helix_segment *segment = twitch_helix_segment_alloc();

	field_spec schema[] = {
		{
			.name = "duration",
			.dest = &segment->duration,
			.parser = &parse_int
		},
		{
			.name = "offset",
			.dest = &segment->offset,
			.parser = &parse_int
		}
	};
	parse_entity(object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)segment;
}

void parse_helix_segment_list(void *dest, json_value *value) {
	if (value->type == json_array) {
		int size = 0;
		twitch_helix_segment_list *list = twitch_helix_segment_list_alloc();

		twitch_helix_segment **items =
			(twitch_helix_segment **)parse_json_array(
				value,
				&size,
				&parse_helix_segment
			);
		list->count = size;
		list->items = items;

		*((void **)dest) = list;
	}
}

void *parse_helix_video(json_value *object) {
	twitch_helix_video *video = twitch_helix_video_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &video->id,
			.parser = &parse_string
		},
		{
			.name = "stream_id",
			.dest = &video->stream_id,
			.parser = &parse_string
		},
		{
			.name = "user_id",
			.dest = &video->user_id,
			.parser = &parse_string
		},
		{
			.name = "user_login",
			.dest = &video->user_login,
			.parser = &parse_string
		},
		{
			.name = "user_name",
			.dest = &video->user_name,
			.parser = &parse_string
		},
		{
			.name = "title",
			.dest = &video->title,
			.parser = &parse_string
		},
		{
			.name = "description",
			.dest = &video->description,
			.parser = &parse_string
		},
		{
			.name = "created_at",
			.dest = &video->created_at,
			.parser = &parse_string
		},
		{
			.name = "published_at",
			.dest = &video->published_at,
			.parser = &parse_string
		},
		{
			.name = "url",
			.dest = &video->url,
			.parser = &parse_string
		},
		{
			.name = "thumbnail_url",
			.dest = &video->thumbnail_url,
			.parser = &parse_string
		},
		{
			.name = "viewable",
			.dest = &video->viewable,
			.parser = &parse_string
		},
		{
			.name = "view_count",
			.dest = &video->view_count,
			.parser = &parse_int
		},
		{
			.name = "language",
			.dest = &video->language,
			.parser = &parse_string
		},
		{
			.name = "type",
			.dest = &video->type,
			.parser = &parse_string
		},
		{
			.name = "duration",
			.dest = &video->duration,
			.parser = &parse_string
		},
		{
			.name = "muted_segments",
			.dest = &video->muted_segments,
			.parser = &parse_helix_segment_list
		},
	};
	parse_entity(object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)video;
}

/** Categories/games search **/

void *parse_helix_category(json_value *object) {
	twitch_helix_category *category = twitch_helix_category_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &category->id,
			.parser = &parse_string
		},
		{
			.name = "name",
			.dest = &category->name,
			.parser = &parse_string
		},
		{
			.name = "box_art_url",
			.dest = &category->box_art_url,
			.parser = &parse_string
		}
	};
	parse_entity(object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)category;
}

/** Channels search **/

void *parse_helix_channel_search_item(json_value *object) {
	twitch_helix_channel_search_item *item =
		twitch_helix_channel_search_item_alloc();

	field_spec schema[] = {
		{
			.name = "id",
			.dest = &item->id,
			.parser = &parse_string
		},
		{
			.name = "game_id",
			.dest = &item->game_id,
			.parser = &parse_string
		},
		{
			.name = "game_name",
			.dest = &item->game_name,
			.parser = &parse_string
		},
		{
			.name = "display_name",
			.dest = &item->display_name,
			.parser = &parse_string
		},
		{
			.name = "broadcaster_language",
			.dest = &item->broadcaster_language,
			.parser = &parse_string
		},
		{
			.name = "broadcaster_login",
			.dest = &item->broadcaster_login,
			.parser = &parse_string
		},
		{
			.name = "is_live",
			.dest = &item->is_live,
			.parser = &parse_bool
		},
		{
			.name = "thumbnail_url",
			.dest = &item->thumbnail_url,
			.parser = &parse_string
		},
		{
			.name = "title",
			.dest = &item->title,
			.parser = &parse_string
		},
		{
			.name = "started_at",
			.dest = &item->started_at,
			.parser = &parse_string
		},
		{
			.name = "tags",
			.dest = &item->tags,
			.parser = &make_string_list
		},
	};
	parse_entity(object, sizeof(schema)/sizeof(field_spec), schema);

	return (void *)item;
}
