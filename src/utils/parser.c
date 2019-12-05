#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "parser.h"
#include "string_utils.h"

/** JSON array parser **/

void **parse_json_array(json_value *value, int *count, void *(*parser)(json_value *)) {
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
        (*(specs[spec_ind].parser))(specs[spec_ind].dest, src->u.object.values[prop_ind].value);
      }
    }
  }
}

/** Art **/

twitch_art *parse_art(json_value *value) {
  twitch_art *art = twitch_art_alloc();

  field_spec schema[] = {
    { .name = "large", .dest = &art->large, .parser = &parse_string },
    { .name = "medium", .dest = &art->medium, .parser = &parse_string },
    { .name = "small", .dest = &art->small, .parser = &parse_string },
    { .name = "template", .dest = &art->template, .parser = &parse_string },
  }; 
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)art;
}

void _parse_art(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_art(value);
  }
}

/** User **/

void *parse_user(json_value *user_object) {
  twitch_user *user = twitch_user_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &user->id, .parser = &parse_id },
    { .name = "display_name", .dest = &user->display_name, .parser = &parse_string },
    { .name = "name", .dest = &user->name, .parser = &parse_string },
    { .name = "logo", .dest = &user->logo, .parser = &parse_string },
    { .name = "bio", .dest = &user->bio, .parser = &parse_string },
    { .name = "type", .dest = &user->type, .parser = &parse_string },
    { .name = "created_at", .dest = &user->created_at, .parser = &parse_string },
    { .name = "updated_at", .dest = &user->updated_at, .parser = &parse_string }
  }; 
  parse_entity(user_object, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)user;
}

void _parse_user(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_user(value);
  }
}

/** Channel **/

void *parse_channel(json_value *channel_object) {
  twitch_channel *channel = twitch_channel_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &channel->id, .parser = &parse_id },
    { .name = "display_name", .dest = &channel->display_name, .parser = &parse_string },
    { .name = "name", .dest = &channel->name, .parser = &parse_string },
    { .name = "game", .dest = &channel->game, .parser = &parse_string },
    { .name = "status", .dest = &channel->status, .parser = &parse_string },
    { .name = "logo", .dest = &channel->logo, .parser = &parse_string },
    { .name = "profile_banner", .dest = &channel->logo, .parser = &parse_string },
    { .name = "created_at", .dest = &channel->created_at, .parser = &parse_string },
    { .name = "updated_at", .dest = &channel->updated_at, .parser = &parse_string },
    { .name = "language", .dest = &channel->language, .parser = &parse_string },
    { .name = "broadcaster_language", .dest = &channel->broadcaster_language, .parser = &parse_string },
    { .name = "url", .dest = &channel->url, .parser = &parse_string },
    { .name = "video_banner", .dest = &channel->video_banner, .parser = &parse_string },
    { .name = "followers", .dest = &channel->followers, .parser = &parse_int },
    { .name = "views", .dest = &channel->views, .parser = &parse_int },
    { .name = "mature", .dest = &channel->mature, .parser = &parse_bool },
    { .name = "partner", .dest = &channel->partner, .parser = &parse_bool },
    { .name = "profile_banner_background_color", .dest = &channel->profile_banner_background_color, .parser = &parse_string },
    { .name = "private_video", .dest = &channel->private_video, .parser = &parse_bool },
    { .name = "privacy_options_enabled", .dest = &channel->privacy_options_enabled, .parser = &parse_bool },
    { .name = "broadcaster_type", .dest = &channel->broadcaster_type, .parser = &parse_string },
    { .name = "broadcaster_software", .dest = &channel->broadcaster_software, .parser = &parse_string },
  };
  parse_entity(channel_object, sizeof(schema)/sizeof(schema[0]), schema);

  return (void *)channel;
}

void _parse_channel(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_channel(value);
  }
}

void _parse_channel_list(void *dest, json_value *value) {
  if (value->type == json_array) {
    int size = 0;
    twitch_channel_list *list = twitch_channel_list_alloc();

    twitch_channel **channels = (twitch_channel **)parse_json_array(value, &size, &parse_channel);
    list->count = size;
    list->items = channels;

    *((void **)dest) = list;
  }
}

/** Follow **/

void *parse_follow(json_value *follow_object) {
  twitch_follow *follow = twitch_follow_alloc();

  field_spec schema[] = {
    { .name = "created_at", .dest = &follow->created_at, .parser = &parse_string },
    { .name = "notifications", .dest = &follow->notifications, .parser = &parse_bool },
    { .name = "channel", .dest = &follow->channel, .parser = &_parse_channel }
  };
  parse_entity(follow_object, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)follow;
}

/** Streams **/

void *parse_stream(json_value *stream_object) {
  twitch_stream *stream = twitch_stream_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &stream->id, .parser = &parse_id },
    { .name = "created_at", .dest = &stream->created_at, .parser = &parse_string },
    { .name = "average_fps", .dest = &stream->average_fps, .parser = &parse_double },
    { .name = "channel", .dest = &stream->channel, .parser = &_parse_channel },
    { .name = "delay", .dest = &stream->delay, .parser = &parse_int },
    { .name = "video_height", .dest = &stream->video_height, .parser = &parse_int },
    { .name = "viewers", .dest = &stream->viewers, .parser = &parse_int },
    { .name = "is_playlist", .dest = &stream->is_playlist, .parser = &parse_bool },
    { .name = "game", .dest = &stream->game, .parser = &parse_string },
    { .name = "preview", .dest = &stream->preview, .parser = &_parse_art },
  };
  parse_entity(stream_object, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)stream;
}

void _parse_stream(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_stream(value);
  }
}

void *parse_summary(json_value *summary_object) {
  twitch_summary *summary = twitch_summary_alloc();

  field_spec schema[] = {
    { .name = "channels", .dest = &summary->channels, .parser = &parse_int },
    { .name = "viewers", .dest = &summary->viewers, .parser = &parse_int },
  };
  parse_entity(summary_object, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)summary;
}

void *parse_featured_stream(json_value *featured_object) {
  twitch_featured_stream *stream = twitch_featured_stream_alloc();

  field_spec schema[] = {
    { .name = "image", .dest = &stream->image, .parser = &parse_string },
    { .name = "priority", .dest = &stream->priority, .parser = &parse_int },
    { .name = "scheduled", .dest = &stream->scheduled, .parser = &parse_bool },
    { .name = "sponsored", .dest = &stream->sponsored, .parser = &parse_bool },
    { .name = "text", .dest = &stream->text, .parser = &parse_string },
    { .name = "title", .dest = &stream->title, .parser = &parse_string },
    { .name = "stream", .dest = &stream->stream, .parser = &_parse_stream },
  };
  parse_entity(featured_object, sizeof(schema)/sizeof(field_spec), schema);

  return stream;
}

void *parse_game(json_value *value) {
  twitch_game *game = twitch_game_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &game->id, .parser = &parse_id },
    { .name = "box", .dest = &game->box, .parser = &_parse_art },
    { .name = "giantbomb_id", .dest = &game->giantbomb_id, .parser = &parse_int },
    { .name = "logo", .dest = &game->logo, .parser = &_parse_art },
    { .name = "name", .dest = &game->name, .parser = &parse_string },
    { .name = "popularity", .dest = &game->popularity, .parser = &parse_int },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)game;
}

void _parse_game(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_game(value);
  }
}

void *parse_top_game(json_value *value) {
  twitch_top_game *game = twitch_top_game_alloc();

  field_spec schema[] = {
    { .name = "game", .dest = &game->game, .parser = &_parse_game },
    { .name = "channels", .dest = &game->channels, .parser = &parse_int },
    { .name = "viewers", .dest = &game->viewers, .parser = &parse_int },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)game;
}

void *parse_follower(json_value *value) {
  twitch_follower *follower = twitch_follower_alloc();

  field_spec schema[] = {
    { .name = "created_at", .dest = &follower->created_at, .parser = &parse_string },
    { .name = "notifications", .dest = &follower->notifications, .parser = &parse_int },
    { .name = "user", .dest = &follower->user, .parser = &_parse_user },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)follower;
}

void *parse_team(json_value *value) {
  twitch_team *team = twitch_team_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &team->id, .parser = &parse_id },
    { .name = "created_at", .dest = &team->created_at, .parser = &parse_string },
    { .name = "updated_at", .dest = &team->updated_at, .parser = &parse_string },
    { .name = "background", .dest = &team->background, .parser = &parse_string },
    { .name = "logo", .dest = &team->logo, .parser = &parse_string },
    { .name = "banner", .dest = &team->banner, .parser = &parse_string },
    { .name = "info", .dest = &team->info, .parser = &parse_string },
    { .name = "display_name", .dest = &team->display_name, .parser = &parse_string },
    { .name = "name", .dest = &team->name, .parser = &parse_string },
    { .name = "users", .dest = &team->users, .parser = &_parse_channel_list },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)team;
}

void *parse_community(json_value *value) {
  twitch_community *community = twitch_community_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &community->id, .parser = &parse_string },
    { .name = "owner_id", .dest = &community->owner_id, .parser = &parse_id },
    { .name = "avatar_image_url", .dest = &community->avatar_image_url, .parser = &parse_string },
    { .name = "cover_image_url", .dest = &community->cover_image_url, .parser = &parse_string },
    { .name = "description", .dest = &community->description, .parser = &parse_string },
    { .name = "description_html", .dest = &community->description_html, .parser = &parse_string },
    { .name = "rules", .dest = &community->rules, .parser = &parse_string },
    { .name = "rules_html", .dest = &community->rules_html, .parser = &parse_string },
    { .name = "name", .dest = &community->name, .parser = &parse_string },
    { .name = "display_name", .dest = &community->display_name, .parser = &parse_string },
    { .name = "summary", .dest = &community->summary, .parser = &parse_string },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)community;
}

void *parse_resolutions(json_value *value) {
  twitch_resolutions *resolutions = twitch_resolutions_alloc();
  
  field_spec schema[] = {
    { .name = "chunked", .dest = &resolutions->chunked, .parser = &parse_string },
    { .name = "high", .dest = &resolutions->high, .parser = &parse_string },
    { .name = "medium", .dest = &resolutions->medium, .parser = &parse_string },
    { .name = "low", .dest = &resolutions->low, .parser = &parse_string },
    { .name = "mobile", .dest = &resolutions->mobile, .parser = &parse_string },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)resolutions;
}

void _parse_resolutions(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_resolutions(value);
  }
}

void *parse_fps(json_value *value) {
  twitch_fps *fps = twitch_fps_alloc();

  field_spec schema[] = {
    { .name = "chunked", .dest = &fps->chunked, .parser = &parse_double },
    { .name = "high", .dest = &fps->high, .parser = &parse_double },
    { .name = "medium", .dest = &fps->medium, .parser = &parse_double },
    { .name = "low", .dest = &fps->low, .parser = &parse_double },
    { .name = "mobile", .dest = &fps->mobile, .parser = &parse_double },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)fps;
}

void _parse_fps(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_fps(value);
  }
}

void *parse_channel_ref(json_value *value) {
  twitch_channel_ref *ref = twitch_channel_ref_alloc();
  
  field_spec schema[] = {
    { .name = "_id", .dest = &ref->id, .parser = &parse_id },
    { .name = "name", .dest = &ref->name, .parser = &parse_string },
    { .name = "display_name", .dest = &ref->display_name, .parser = &parse_string },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)ref;
}

void _parse_channel_ref(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_channel_ref(value);
  }
}

void *parse_thumbnail(json_value *value) {
  twitch_thumbnail *thumb = twitch_thumbnail_alloc();
  
  field_spec schema[] = {
    { .name = "type", .dest = &thumb->type, .parser = &parse_string },
    { .name = "url", .dest = &thumb->url, .parser = &parse_string },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)thumb;
}

void _parse_thumbnail(void *dest, json_value *value) {
  if (value->type == json_object) {
    *((void **)dest) = parse_thumbnail(value);
  }
}

void _parse_thumbnail_list(void *dest, json_value *value) {
  if (value->type == json_array) {
    int size = 0;
    twitch_thumbnail **thumbs = (twitch_thumbnail **)parse_json_array(value, &size, &parse_thumbnail);

    twitch_thumbnail_list *list = twitch_thumbnail_list_alloc();
    list->count = size;
    list->items = thumbs;

     *((void **)dest) = list;
  }
}

void _parse_video_thumbnails(void *dest, json_value *value) {
  if (value->type == json_object) {
    twitch_video_thumbnails *thumbs = twitch_video_thumbnails_alloc();

    field_spec schema[] = {
      { .name = "large", .dest = &thumbs->large, .parser = &_parse_thumbnail_list },
      { .name = "medium", .dest = &thumbs->medium, .parser = &_parse_thumbnail_list },
      { .name = "small", .dest = &thumbs->small, .parser = &_parse_thumbnail_list },
      { .name = "template", .dest = &thumbs->template, .parser = &_parse_thumbnail_list },
    };
    parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

    *((void **)dest) = thumbs;
  }
}

void *parse_video(json_value *value) {
  twitch_video *video = twitch_video_alloc();

  field_spec schema[] = {
    { .name = "_id", .dest = &video->id, .parser = &parse_string },
    { .name = "broadcast_id", .dest = &video->broadcast_id, .parser = &parse_id },
    { .name = "channel", .dest = &video->channel, .parser = &_parse_channel_ref },
    { .name = "created_at", .dest = &video->created_at, .parser = &parse_string },
    { .name = "description", .dest = &video->description, .parser = &parse_string },
    { .name = "description_html", .dest = &video->description_html, .parser = &parse_string },
    { .name = "fps", .dest = &video->fps, .parser = &_parse_fps },
    { .name = "game", .dest = &video->game, .parser = &parse_string },
    { .name = "language", .dest = &video->language, .parser = &parse_string },
    { .name = "length", .dest = &video->length, .parser = &parse_int },
    { .name = "preview", .dest = &video->preview, .parser = &_parse_art },
    { .name = "published_at", .dest = &video->published_at, .parser = &parse_string },
    { .name = "resolutions", .dest = &video->resolutions, .parser = &_parse_resolutions },
    { .name = "status", .dest = &video->status, .parser = &parse_string },
    { .name = "tag_list", .dest = &video->tag_list, .parser = &parse_string },
    { .name = "thumbnails", .dest = &video->thumbnails, .parser = &_parse_video_thumbnails },
    { .name = "title", .dest = &video->title, .parser = &parse_string },
    { .name = "url", .dest = &video->url, .parser = &parse_string },
    { .name = "viewable", .dest = &video->viewable, .parser = &parse_string },
    { .name = "viewable_at", .dest = &video->viewable_at, .parser = &parse_string },
    { .name = "views", .dest = &video->views, .parser = &parse_int },
  };
  parse_entity(value, sizeof(schema)/sizeof(field_spec), schema);

  return (void *)video;
}

