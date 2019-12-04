#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "parser.h"
#include "string_utils.h"

/** JSON parsers **/

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

/** Parsing entities **/

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
        (*specs[spec_ind].parser)(specs[spec_ind].dest, src->u.object.values[prop_ind].value);
      }
    }
  }
}

/** Art **/

twitch_art *parse_art(json_value *value) {
  twitch_art *art = twitch_art_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "large") == 0) {
      // Large art.
      if (value->u.object.values[prop_ind].value->type == json_string) {
        art->large = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "medium") == 0) {
      // Medium art.
      if (value->u.object.values[prop_ind].value->type == json_string) {
        art->medium = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "small") == 0) {
      // Small art.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        art->small = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "template") == 0) {
      // Template.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        art->template = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    }
  }

  return (void *)art;
}

void _parse_art(void *dest, json_value *value) {
  *((void **)dest) = parse_art(value);
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
  parse_entity(user_object, sizeof(schema)/sizeof(schema[0]), schema);

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

/** Follow **/

void *parse_follow(json_value *follow_object) {
  twitch_follow *follow = twitch_follow_alloc();

  field_spec schema[] = {
    { .name = "created_at", .dest = &follow->created_at, .parser = &parse_string },
    { .name = "notifications", .dest = &follow->notifications, .parser = &parse_bool },
    { .name = "channel", .dest = &follow->channel, .parser = &_parse_channel }
  };
  parse_entity(follow_object, sizeof(schema)/sizeof(schema[0]), schema);

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
  parse_entity(stream_object, sizeof(schema)/sizeof(schema[0]), schema);

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
  parse_entity(summary_object, sizeof(schema)/sizeof(schema[0]), schema);

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
  parse_entity(featured_object, sizeof(schema)/sizeof(schema[0]), schema);

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
  parse_entity(value, sizeof(schema)/sizeof(schema[0]), schema);

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
  parse_entity(value, sizeof(schema)/sizeof(schema[0]), schema);

  return (void *)game;
}

void *parse_follower(json_value *value) {
  twitch_follower *follower = twitch_follower_alloc();

  field_spec schema[] = {
    { .name = "created_at", .dest = &follower->created_at, .parser = &parse_string },
    { .name = "notifications", .dest = &follower->notifications, .parser = &parse_int },
    { .name = "user", .dest = &follower->user, .parser = &_parse_user },
  };
  parse_entity(value, sizeof(schema)/sizeof(schema[0]), schema);

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
  };
  parse_entity(value, sizeof(schema)/sizeof(schema[0]), schema);

  return (void *)team;
}

void *parse_community(json_value *value) {
  twitch_community *community = twitch_community_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "owner_id") == 0) {
      // Owner ID.
      community->owner_id = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      community->id = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "avatar_image_url") == 0) {
      // Avatar image URL.
      community->avatar_image_url = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "cover_image_url") == 0) {
      // Cover image URL.
      community->cover_image_url = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "description") == 0) {
      // Description.
      community->description = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "description_html") == 0) {
      // Description HTML.
      community->description_html = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "rules_html") == 0) {
      // Rules HTML.
      community->rules_html = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "rules") == 0) {
      // Rules.
      community->rules = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      community->name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "display_name") == 0) {
      // Display name.
      community->display_name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "summary") == 0) {
      // Summary.
      community->summary = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

  return (void *)community;
}

void *parse_resolutions(json_value *value) {
  twitch_resolutions *resolutions = twitch_resolutions_alloc();
  
  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "chunked") == 0) {
      // Chunked.
      resolutions->chunked = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "high") == 0) {
      // High.
      resolutions->high = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "medium") == 0) {
      // Medium.
      resolutions->medium = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "low") == 0) {
      // Low.
      resolutions->low = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "mobile") == 0) {
      // Mobile.
      resolutions->mobile = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

  return (void *)resolutions;
}

void *parse_fps(json_value *value) {
  twitch_fps *fps = twitch_fps_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "chunked") == 0) {
      // Chunked.
      fps->chunked = value->u.object.values[prop_ind].value->u.dbl;
    } else if (strcmp(value->u.object.values[prop_ind].name, "high") == 0) {
      // High.
      fps->high = value->u.object.values[prop_ind].value->u.dbl;
    } else if (strcmp(value->u.object.values[prop_ind].name, "low") == 0) {
      // Low.
      fps->low = value->u.object.values[prop_ind].value->u.dbl;
    } else if (strcmp(value->u.object.values[prop_ind].name, "medium") == 0) {
      // Medium.
      fps->medium = value->u.object.values[prop_ind].value->u.dbl;
    } else if (strcmp(value->u.object.values[prop_ind].name, "mobile") == 0) {
      // Mobile.
      fps->mobile = value->u.object.values[prop_ind].value->u.dbl;
    }
  }

  return (void *)fps;
}

void *parse_channel_ref(json_value *value) {
  twitch_channel_ref *ref = twitch_channel_ref_alloc();
  
  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      ref->id = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "display_name") == 0) {
      // Display name.
      ref->display_name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      ref->name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

  return (void *)ref;
}

void *parse_thumbnail(json_value *value) {
  twitch_thumbnail *thumb = twitch_thumbnail_alloc();
  
  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "type") == 0) {
      // Type.
      thumb->type = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "url") == 0) {
      // URL.
      thumb->url = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

  return (void *)thumb;
}

void *parse_thumbnail_list(json_value *value) {
  int size = 0;
  twitch_thumbnail **thumbs = (twitch_thumbnail **)parse_json_array(value, &size, &parse_thumbnail);

  twitch_thumbnail_list *list = twitch_thumbnail_list_alloc();
  list->count = size;
  list->items = thumbs;

  return (void *)list;
}

void *parse_video_thumbnails(json_value *value) {
  twitch_video_thumbnails *thumbs = twitch_video_thumbnails_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "large") == 0) {
      // Large.
      thumbs->large = parse_thumbnail_list(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "medium") == 0) {
      // Medium.
      thumbs->medium = parse_thumbnail_list(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "small") == 0) {
      // Small.
      thumbs->small = parse_thumbnail_list(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "template") == 0) {
      // Template.
      thumbs->template = parse_thumbnail_list(value->u.object.values[prop_ind].value);
    }
  }

  return (void *)thumbs;
}

void *parse_video(json_value *value) {
  twitch_video *video = twitch_video_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      video->id = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "broadcast_id") == 0) {
      // Broadcast ID.
      if (value->u.object.values[prop_ind].value->type == json_integer) {
        video->broadcast_id = value->u.object.values[prop_ind].value->u.integer;
      } else if (value->u.object.values[prop_ind].value->type == json_string) {
        video->broadcast_id = strtoimax(value->u.object.values[prop_ind].value->u.string.ptr, NULL, 10);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "channel") == 0) {
      // Channel.
      video->channel = parse_channel_ref(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date.
      video->created_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "description") == 0) {
      // Description.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        video->description = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "description_html") == 0) {
      // HTML description.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        video->description_html = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "fps") == 0) {
      // FPS specs.
      video->fps = parse_fps(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "game") == 0) {
      // Game.
      video->game = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "language") == 0) {
      // Language.
      video->language = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "length") == 0) {
      // Length.
      video->length = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "preview") == 0) {
      // Preview.
      video->preview = parse_art(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "published_at") == 0) {
      // Published At date.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        video->published_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "resolutions") == 0) {
      // Resolutions.
      video->resolutions = parse_resolutions(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "status") == 0) {
      // Status.
      video->status = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "tag_list") == 0) {
      // Tag list.
      video->tag_list = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "thumbnails") == 0) {
      // Thumbnails.
      video->thumbnails = parse_video_thumbnails(value->u.object.values[prop_ind].value);
    } else if (strcmp(value->u.object.values[prop_ind].name, "title") == 0) {
      // Title.
      video->title = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "url") == 0) {
      // URL.
      video->url = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "viewable") == 0) {
      // Viewable.
      video->viewable = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "viewable_at") == 0) {
      // Viewable at.
      if (value->u.object.values[prop_ind].value->type != json_null) {
        video->viewable_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "views") == 0) {
      // Views.
      video->views = value->u.object.values[prop_ind].value->u.integer;
    } 
  }

  return (void *)video;
}
