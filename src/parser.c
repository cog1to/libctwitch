#include <stdlib.h>
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

void *parse_user(json_value *user_object) {
  twitch_user *user = twitch_user_alloc();

  for (int prop_ind = 0; prop_ind < user_object->u.object.length; prop_ind++) {
    if (strcmp(user_object->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      user->id = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "display_name") == 0) {
      // Display name.
      user->display_name = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      user->name = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "logo") == 0) {
      // Logo.
      if (user_object->u.object.values[prop_ind].value->type == json_string) {
        user->logo = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "bio") == 0) {
      // Logo.
      if (user_object->u.object.values[prop_ind].value->type == json_string) {
        user->bio = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "type") == 0) {
      // Type.
      user->type = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date string.
      user->created_at = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(user_object->u.object.values[prop_ind].name, "updated_at") == 0) {
      // Updated At date string.
      user->updated_at = immutable_string_copy(user_object->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

  return (void *)user;
}

void *parse_channel(json_value *channel_object) {
  twitch_channel *channel = twitch_channel_alloc();

  for (int prop_ind = 0; prop_ind < channel_object->u.object.length; prop_ind++) {
    if (strcmp(channel_object->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      if (channel_object->u.object.values[prop_ind].value->type == json_integer) {
        channel->id = channel_object->u.object.values[prop_ind].value->u.integer;
      } else if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->id = strtoimax(channel_object->u.object.values[prop_ind].value->u.string.ptr, NULL, 10);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "display_name") == 0) {
      // Display name.
      channel->display_name = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      channel->name = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "game") == 0) {
      // Game.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->game = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "status") == 0) {
      // Status.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->status = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "logo") == 0) {
      // Logo.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->logo = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "profile_banner") == 0) {
      // Profile banner.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->profile_banner = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date string.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->created_at = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "updated_at") == 0) {
      // Updated At date string.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->updated_at = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "language") == 0) {
      // Language.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->language = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "broadcaster_language") == 0) {
      // Broadcaster language.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->broadcaster_language = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "url") == 0) {
      // URL.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->url = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "video_banner") == 0) {
      // Video banner.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->video_banner = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "followers") == 0) {
      // Followers.
      channel->followers = channel_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "views") == 0) {
      // Views.
      channel->views = channel_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "mature") == 0) {
      // Mature flag.
      channel->mature = channel_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "partner") == 0) {
      // Partner flag.
      channel->partner = channel_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "profile_banner_background_color") == 0) {
      // Profile banner background color.
      if (channel_object->u.object.values[prop_ind].value->type == json_string) {
        channel->profile_banner_background_color = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "private_video") == 0) {
      // Private video flag.
      channel->private_video = channel_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "privacy_options_enabled") == 0) {
      // Privacy options enabled flag.
      channel->privacy_options_enabled = channel_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "broadcaster_type") == 0) {
      // Broadcaster type.
      if (channel_object->u.object.values[prop_ind].value->type != json_null) {
        channel->broadcaster_type = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(channel_object->u.object.values[prop_ind].name, "broadcaster_software") == 0) {
      // Broadcaster sowftware.
      if (channel_object->u.object.values[prop_ind].value->type != json_null) {
        channel->broadcaster_software = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    }

  }

  return (void *)channel;
}

void *parse_follow(json_value *follow_object) {
  twitch_follow *follow = malloc(sizeof(twitch_follow));

  for (int prop_ind = 0; prop_ind < follow_object->u.object.length; prop_ind++) {
    if (strcmp(follow_object->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date.
      follow->created_at = immutable_string_copy(follow_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(follow_object->u.object.values[prop_ind].name, "notifications") == 0) {
      // Notifications.
      follow->notifications = follow_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(follow_object->u.object.values[prop_ind].name, "channel") == 0) {
      // Channel.
      follow->channel = parse_channel(follow_object->u.object.values[prop_ind].value);
    }
  }

  return (void *)follow;
}

void *parse_stream(json_value *stream_object) {
  twitch_stream *stream = twitch_stream_alloc();

  for (int prop_ind = 0; prop_ind < stream_object->u.object.length; prop_ind++) {
    if (strcmp(stream_object->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      stream->id = stream_object->u.object.values[prop_ind].value->u.integer;
    } if (strcmp(stream_object->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date.
      stream->created_at = immutable_string_copy(stream_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "average_fps") == 0) {
      // Average FPS.
      stream->average_fps = stream_object->u.object.values[prop_ind].value->u.dbl;
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "channel") == 0) {
      // Channel.
      stream->channel = parse_channel(stream_object->u.object.values[prop_ind].value);
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "delay") == 0) {
      // Delay.
      stream->delay = stream_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "video_height") == 0) {
      // Video height.
      stream->video_height = stream_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "viewers") == 0) {
      // Viewers.
      stream->viewers = stream_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "is_playlist") == 0) {
      // Is playlist flag.
      stream->is_playlist = stream_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "game") == 0) {
      // Game.
      stream->game = immutable_string_copy(stream_object->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(stream_object->u.object.values[prop_ind].name, "preview") == 0) {
      // TODO: Preview parsing.
    }
  }

  return stream;
}

void *parse_summary(json_value *summary_object) {
  twitch_summary *summary = twitch_summary_alloc();

  for (int prop_ind = 0; prop_ind < summary_object->u.object.length; prop_ind++) {
    if (strcmp(summary_object->u.object.values[prop_ind].name, "channels") == 0) {
      // Channels.
      summary->channels = summary_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(summary_object->u.object.values[prop_ind].name, "viewers") == 0) {
      // Viewers.
      summary->viewers = summary_object->u.object.values[prop_ind].value->u.integer;
    }
  }

  return (void *)summary;
}

void *parse_featured_stream(json_value *featured_object) {
  twitch_featured_stream *stream = twitch_featured_stream_alloc();

  for (int prop_ind = 0; prop_ind < featured_object->u.object.length; prop_ind++) {
    if (strcmp(featured_object->u.object.values[prop_ind].name, "image") == 0) {
      // Image.
      if (featured_object->u.object.values[prop_ind].value->type == json_string) {
        stream->image = immutable_string_copy(featured_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } if (strcmp(featured_object->u.object.values[prop_ind].name, "priority") == 0) {
      // Priority.
      stream->priority = featured_object->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(featured_object->u.object.values[prop_ind].name, "scheduled") == 0) {
      // Scheduled flag.
      stream->scheduled = featured_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(featured_object->u.object.values[prop_ind].name, "sponsored") == 0) {
      // Sponsored flag.
      stream->sponsored = featured_object->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(featured_object->u.object.values[prop_ind].name, "text") == 0) {
      // Text.
      if (featured_object->u.object.values[prop_ind].value->type == json_string) {
        stream->text = immutable_string_copy(featured_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(featured_object->u.object.values[prop_ind].name, "title") == 0) {
      // Title.
      if (featured_object->u.object.values[prop_ind].value->type == json_string) {
        stream->title = immutable_string_copy(featured_object->u.object.values[prop_ind].value->u.string.ptr);
      }
    } else if (strcmp(featured_object->u.object.values[prop_ind].name, "stream") == 0) {
      // Stream.
      if (featured_object->u.object.values[prop_ind].value->type == json_object) {
        stream->stream = parse_stream(featured_object->u.object.values[prop_ind].value);
      }
    }
  }

  return stream;
}

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

void *parse_game(json_value *value) {
  twitch_game *game = twitch_game_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      game->id = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "box") == 0) {
      // Box art.
      if (value->u.object.values[prop_ind].value->type == json_object) {
        game->box = parse_art(value->u.object.values[prop_ind].value);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "giantbomb_id") == 0) {
      // GiantBomb ID.
      game->giantbomb_id = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "logo") == 0) {
      // Logo.
      if (value->u.object.values[prop_ind].value->type == json_object) {
        game->logo = parse_art(value->u.object.values[prop_ind].value);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      game->name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "popularity") == 0) {
      // Popularity.
      game->popularity = value->u.object.values[prop_ind].value->u.integer;
    }
  }

  return (void *)game;
}

