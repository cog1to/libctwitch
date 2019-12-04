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

/** Top games data **/

twitch_top_game *twitch_top_game_alloc() {
  GENERIC_ALLOC(twitch_top_game)
}

void twitch_top_game_free(twitch_top_game *game) {
  FREE_CUSTOM(game->game, twitch_game_free)
  free(game);
}

void twitch_top_game_list_free(int count, twitch_top_game **list) {
  pointer_array_free(count, (void **)list, (void(*))&twitch_top_game_free);
}

/** Channel followers data **/

twitch_follower *twitch_follower_alloc() {
  GENERIC_ALLOC(twitch_follower)
}

void twitch_follower_free(twitch_follower *follower) {
  FREE_CUSTOM(follower->user, twitch_user_free)
  FREE(follower->created_at)
  free(follower);
}

void twitch_follower_list_free(int count, twitch_follower **list) {
  pointer_array_free(count, (void **)list, (void(*))&twitch_follower_free);
}

/** Communities data **/

twitch_community *twitch_community_alloc() {
  GENERIC_ALLOC(twitch_community)
}

void twitch_community_free(twitch_community *community) {
  FREE(community->id)
  FREE(community->name)
  FREE(community->display_name)
  FREE(community->avatar_image_url)
  FREE(community->cover_image_url)
  FREE(community->description)
  FREE(community->description_html)
  FREE(community->rules)
  FREE(community->rules_html)
  FREE(community->language)
  FREE(community->summary)
  free(community);
}

void twitch_community_list_free(int count, twitch_community **list) {
  pointer_array_free(count, (void **)list, (void(*))&twitch_community_free);
}

/** Video data **/

twitch_resolutions *twitch_resolutions_alloc() {
  GENERIC_ALLOC(twitch_resolutions)
}

void twitch_resolutions_free(twitch_resolutions *resolutions) {
  FREE(resolutions->chunked)
  FREE(resolutions->high)
  FREE(resolutions->medium)
  FREE(resolutions->low)
  FREE(resolutions->mobile)
  free(resolutions);
}

twitch_fps *twitch_fps_alloc() {
  GENERIC_ALLOC(twitch_fps)
}

void twitch_fps_free(twitch_fps *fps) {
  free(fps);
}

twitch_channel_ref *twitch_channel_ref_alloc() {
  GENERIC_ALLOC(twitch_channel_ref)
}

void twitch_channel_ref_free(twitch_channel_ref *ref) {
  FREE(ref->name)
  FREE(ref->display_name)
  free(ref);
}

twitch_thumbnail *twitch_thumbnail_alloc() {
  GENERIC_ALLOC(twitch_thumbnail)
}

void twitch_thumbnail_free(twitch_thumbnail *thumbnail) {
  FREE(thumbnail->url)
  FREE(thumbnail->type)
  free(thumbnail);
}

twitch_thumbnail_list *twitch_thumbnail_list_alloc() {
  GENERIC_ALLOC(twitch_thumbnail_list)
}

void twitch_thumbnail_list_free(twitch_thumbnail_list *list) {
  pointer_array_free(list->count, (void **)list->items, (void(*)(void*))&twitch_thumbnail_free);
  free(list);
}

twitch_video_thumbnails *twitch_video_thumbnails_alloc() {
  GENERIC_ALLOC(twitch_video_thumbnails)
}

void twitch_video_thumbnails_free(twitch_video_thumbnails *thumbnails) {
  FREE_CUSTOM(thumbnails->large, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->medium, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->small, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->template, twitch_thumbnail_list_free)
}

twitch_video *twitch_video_alloc() {
  GENERIC_ALLOC(twitch_video)
}

void twitch_video_free(twitch_video *video) {
  FREE(video->id)
  FREE(video->broadcast_type)
  FREE_CUSTOM(video->channel, twitch_channel_ref_free)
  FREE(video->created_at)
  FREE(video->description)
  FREE(video->description_html)
  FREE_CUSTOM(video->fps, twitch_fps_free)
  FREE(video->game)
  FREE(video->language)
  FREE_CUSTOM(video->preview, twitch_art_free)
  FREE(video->published_at)
  FREE_CUSTOM(video->resolutions, twitch_resolutions_free)
  FREE(video->status)
  FREE(video->tag_list)
  FREE(video->title)
  FREE(video->url)
  FREE_CUSTOM(video->thumbnails, twitch_video_thumbnails_free)
  FREE(video->viewable)
  FREE(video->viewable_at)
}

void twitch_video_list_free(int count, twitch_video **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_video_free);
}

/** Teams data **/

twitch_team_user *twitch_team_user_alloc() {
  GENERIC_ALLOC(twitch_team_user)
}

void twitch_team_user_free(twitch_team_user *user) {
  FREE(user->broadcaster_language)
  FREE(user->created_at)
  FREE(user->display_name)
  FREE(user->game)
  FREE(user->language)
  FREE(user->logo)
  FREE(user->name)
  FREE(user->profile_banner)
  FREE(user->profile_banner_background_color)
  FREE(user->status)
  FREE(user->updated_at)
  FREE(user->url)
  FREE(user->video_banner)
  free(user);
}

twitch_team_user_list *twitch_team_user_list_alloc() {
  GENERIC_ALLOC(twitch_team_user_list)
}

void twitch_team_user_list_free(twitch_team_user_list *list) {
  pointer_array_free(list->count, (void **)list->items, (void(*)(void*))&twitch_team_user_free);
  free(list);
}

twitch_team *twitch_team_alloc() {
  GENERIC_ALLOC(twitch_team)
}

void twitch_team_free(twitch_team *team) {
  FREE(team->background)
  FREE(team->banner)
  FREE(team->created_at)
  FREE(team->display_name)
  FREE(team->info)
  FREE(team->logo)
  FREE(team->name)
  FREE(team->updated_at)
  FREE_CUSTOM(team->users, twitch_team_user_list_free)
  free(team);
}

void twitch_team_list_free(int count, twitch_team **list) {
  pointer_array_free(count, (void **)list, (void(*)(void*))&twitch_video_free);
}

