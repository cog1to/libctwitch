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
  twitch_user *user = malloc(sizeof(twitch_user));

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
      channel->status = immutable_string_copy(channel_object->u.object.values[prop_ind].value->u.string.ptr);
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
      stream->average_fps = stream_object->u.object.values[prop_ind].value->u.integer;
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
