#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"
#include "utils/array_utils.h"

/** Code generation **/

#define GENERIC_ALLOC(T) \
  T *instance = calloc(1, sizeof(T)); \
  if (instance == NULL) { \
    fprintf(stderr, "Failed to allocate memory for " #T "");\
    exit(EXIT_FAILURE); \
  } \
  return instance;

#define FREE(prop) \
  if (prop != NULL) { \
    free(prop); \
  }

#define FREE_CUSTOM(prop, deinit) \
  if (prop != NULL) { \
    deinit(prop); \
  }

/** Helper functions **/

void free_if_not_empty(void *ptr, void(*deallocator)(void *)) {
  if (ptr != NULL) {
    (*deallocator)(ptr);
  }
}

/** User data **/

twitch_user *twitch_user_alloc() {
  GENERIC_ALLOC(twitch_user)
}

void twitch_user_free(twitch_user *user) {
  FREE(user->id)
  FREE(user->display_name)
  FREE(user->name)
  FREE(user->created_at)
  FREE(user->updated_at)
  FREE(user->type)
  FREE(user->bio)
  FREE(user->logo)
  free(user);
}

void twitch_users_list_free(twitch_user **list, int count) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_user_free);
}

/** Channel data **/

twitch_channel *twitch_channel_alloc() {
  GENERIC_ALLOC(twitch_channel)
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
  FREE(channel->game)
  FREE(channel->name)
  FREE(channel->status)
  FREE(channel->display_name)
  FREE(channel->language)
  FREE(channel->broadcaster_language)
  FREE(channel->url)
  FREE(channel->video_banner)
  FREE(channel->updated_at)
  FREE(channel->created_at)
  FREE(channel->logo)
  FREE(channel->profile_banner)
  FREE(channel->profile_banner_background_color)
  FREE(channel->broadcaster_software)
  FREE(channel->broadcaster_type)
  free(channel);
}

void twitch_channel_list_free(int count, twitch_channel **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_channel_free);
}

/** Follow data **/

void twitch_follow_free(twitch_follow *follow) {
  FREE(follow->created_at)
  FREE_CUSTOM(follow->channel, twitch_channel_free);
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
  GENERIC_ALLOC(twitch_stream)
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
  FREE(stream->created_at)
  FREE(stream->game)
  FREE_CUSTOM(stream->channel, twitch_channel_free);
  free(stream);
}

void twitch_stream_list_free(int count, twitch_stream **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_stream_free);
}

twitch_summary *twitch_summary_alloc() {
  GENERIC_ALLOC(twitch_summary)
}

void twitch_summary_free(twitch_summary *summary) {
  free(summary);
}

twitch_featured_stream *twitch_featured_stream_alloc() {
  GENERIC_ALLOC(twitch_featured_stream)
}

void twitch_featured_stream_free(twitch_featured_stream *stream) {
  FREE(stream->image)
  FREE(stream->text)
  FREE(stream->title)
  FREE_CUSTOM(stream->stream, twitch_stream_free);
  free(stream);
}

void twitch_featured_stream_list_free(int count, twitch_featured_stream **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_featured_stream_free);
}

/** Art data **/

twitch_art *twitch_art_alloc() {
  GENERIC_ALLOC(twitch_art)
}

void twitch_art_free(twitch_art *art) {
  FREE(art->large)
  FREE(art->medium)
  FREE(art->small)
  FREE(art->template)
  free(art);
}

/** Game data  **/

twitch_game *twitch_game_alloc() {
  GENERIC_ALLOC(twitch_game)
}

void twitch_game_free(twitch_game *game) {
  FREE_CUSTOM(game->box, twitch_art_free)
  FREE_CUSTOM(game->logo, twitch_art_free)
  FREE(game->name)
  FREE(game->localized_name)
  FREE(game->locale)
  free(game);
}

void twitch_game_list_free(int count, twitch_game **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_game_free);
}

