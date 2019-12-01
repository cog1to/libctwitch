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

void *parse_top_game(json_value *value) {
  twitch_top_game *game = twitch_top_game_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "game") == 0) {
      // Game.
      if (value->u.object.values[prop_ind].value->type == json_object) {
        game->game = parse_game(value->u.object.values[prop_ind].value);
      }
    } else if (strcmp(value->u.object.values[prop_ind].name, "channels") == 0) {
      // Channels.
      game->channels = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "viewers") == 0) {
      // Viewers.
      game->viewers = value->u.object.values[prop_ind].value->u.integer;
    }
  }

  return (void *)game;
}

void *parse_follower(json_value *value) {
  twitch_follower *follower = twitch_follower_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date.
      follower->created_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "notifications") == 0) {
      // Notifications.
      follower->notifications = value->u.object.values[prop_ind].value->u.boolean;
    } else if (strcmp(value->u.object.values[prop_ind].name, "user") == 0) {
      // User.
      follower->user = parse_user(value->u.object.values[prop_ind].value);
    }
  }

  return (void *)follower;
}

void *parse_team(json_value *value) {
  twitch_team *team = twitch_team_alloc();

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "_id") == 0) {
      // ID.
      team->id = value->u.object.values[prop_ind].value->u.integer;
    } else if (strcmp(value->u.object.values[prop_ind].name, "created_at") == 0) {
      // Created At date.
      team->created_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "updated_at") == 0) {
      // Updated At date.
      team->updated_at = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "background") == 0) {
      // Background.
      team->background = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "logo") == 0) {
      // Logo.
      team->logo = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "banner") == 0) {
      // Banner.
      team->banner = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "name") == 0) {
      // Name.
      team->name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "display_name") == 0) {
      // Display name.
      team->display_name = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    } else if (strcmp(value->u.object.values[prop_ind].name, "info") == 0) {
      // Info.
      team->info = immutable_string_copy(value->u.object.values[prop_ind].value->u.string.ptr);
    }
  }

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
