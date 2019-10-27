#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "array_utils.h"

/** User data **/

void twitch_user_free(twitch_user *user) {
  free(user->id);
  free(user->display_name);
  free(user->name);
  free(user);
}

/** Channel data **/

twitch_channel *twitch_channel_alloc() {
  twitch_channel *channel = malloc(sizeof(twitch_channel));
  memset(channel, 0, sizeof(twitch_channel));
  return channel;
}

twitch_channel *twitch_channel_init(long long int id, char *game, char *name, char *status, char *display_name) {
  twitch_channel *channel = twitch_channel_alloc();

  channel->id = id;

  channel->game = malloc(strlen(game) + 1);
  strcpy(channel->game, game);

  channel->name = malloc(strlen(name) + 1);
  strcpy(channel->name, name);

  channel->status = malloc(strlen(status) + 1);
  strcpy(channel->status, status);

  channel->display_name = malloc(strlen(display_name) + 1);
  strcpy(channel->display_name, display_name);

  return channel;
}

void twitch_channel_free(twitch_channel *channel) {
  if (channel->game != NULL) {
    free(channel->game);
  }

  if (channel->name != NULL) {
    free(channel->name);
  }

  if (channel->status != NULL) {
    free(channel->status);
  }

  if (channel->display_name != NULL) {
    free(channel->display_name);
  }

  free(channel);
}

/** Follow data **/

void twitch_follow_free(twitch_follow *follow) {
  free(follow->created_at);
  twitch_channel_free(follow->channel);
  free(follow);
}

void twitch_follows_list_free(twitch_follow **list, int count) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_follow_free);
}

twitch_follow *twitch_follow_init(char *created_at, int notifications, twitch_channel *channel) {
  twitch_follow *follow = malloc(sizeof(twitch_follow));

  follow->created_at = malloc(strlen(created_at) + 1);
  strcpy(follow->created_at, created_at);

  follow->notifications = notifications;

  follow->channel = channel;

  return follow;
}

twitch_stream *twitch_stream_alloc() {
  twitch_stream *stream = malloc(sizeof(twitch_stream));
  memset(stream, 0, sizeof(twitch_stream));
  return stream;
}

twitch_stream *twitch_stream_init(long long int id, int average_fps, twitch_channel *channel, char *created_at, int delay, char *game, int is_playlist, int video_height, int viewers) {
  twitch_stream *stream = twitch_stream_alloc();

  stream->id = id;

  stream->average_fps = average_fps;
  stream->channel = channel;

  stream->created_at = malloc(strlen(created_at) + 1);
  strcpy(stream->created_at, created_at);

  stream->delay = delay;
  stream->is_playlist = is_playlist;
  stream->video_height = video_height;
  stream->viewers = viewers;

  if (game != NULL) {
    stream->game = malloc(strlen(game) + 1);
    strcpy(stream->game, game);
  }

  return stream;
}

void twitch_stream_free(twitch_stream *stream) {
  free(stream->created_at);

  if (stream->game != NULL) {
    free(stream->game);
  }

  if (stream->channel != NULL) {
    twitch_channel_free(stream->channel);
  }

  free(stream);
}

void twitch_streams_list_free(twitch_stream **list, int count) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_stream_free);
}

