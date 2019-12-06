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

#define GENERIC_LIST(entity) \
  twitch_##entity##_list *twitch_##entity##_list_alloc() { \
    GENERIC_ALLOC(twitch_##entity##_list) \
  } \
  void twitch_##entity##_list_free(twitch_##entity##_list *list) { \
    pointer_array_free(list->count, (void **)list->items, (void(*)(void*))&twitch_##user##_free); \
    free(list); \
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
  FREE(user->display_name)
  FREE(user->name)
  FREE(user->created_at)
  FREE(user->updated_at)
  FREE(user->type)
  FREE(user->bio)
  FREE(user->logo)
  free(user);
}

GENERIC_LIST(user)

/** Channel data **/

twitch_channel *twitch_channel_alloc() {
  GENERIC_ALLOC(twitch_channel)
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

GENERIC_LIST(channel)

/** Follow data **/

twitch_follow *twitch_follow_alloc() {
  GENERIC_ALLOC(twitch_follow)
}

void twitch_follow_free(twitch_follow *follow) {
  FREE(follow->created_at)
  FREE_CUSTOM(follow->channel, twitch_channel_free);
  free(follow);
}

GENERIC_LIST(follow)

/** Streams **/

twitch_stream *twitch_stream_alloc() {
  GENERIC_ALLOC(twitch_stream)
}

void twitch_stream_free(twitch_stream *stream) {
  FREE(stream->created_at)
  FREE(stream->game)
  FREE_CUSTOM(stream->channel, twitch_channel_free);
  FREE_CUSTOM(stream->preview, twitch_art_free);
  free(stream);
}

GENERIC_LIST(stream)

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

GENERIC_LIST(featured_stream)

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

GENERIC_LIST(game)

/** Top games data **/

twitch_top_game *twitch_top_game_alloc() {
  GENERIC_ALLOC(twitch_top_game)
}

void twitch_top_game_free(twitch_top_game *game) {
  FREE_CUSTOM(game->game, twitch_game_free)
  free(game);
}

GENERIC_LIST(top_game)

/** Channel followers data **/

twitch_follower *twitch_follower_alloc() {
  GENERIC_ALLOC(twitch_follower)
}

void twitch_follower_free(twitch_follower *follower) {
  FREE_CUSTOM(follower->user, twitch_user_free)
  FREE(follower->created_at)
  free(follower);
}

GENERIC_LIST(follower)

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

GENERIC_LIST(community)

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

GENERIC_LIST(thumbnail)

twitch_video_thumbnails *twitch_video_thumbnails_alloc() {
  GENERIC_ALLOC(twitch_video_thumbnails)
}

void twitch_video_thumbnails_free(twitch_video_thumbnails *thumbnails) {
  FREE_CUSTOM(thumbnails->large, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->medium, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->small, twitch_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->template, twitch_thumbnail_list_free)
  free(thumbnails);
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
  free(video);
}

GENERIC_LIST(video)

/** Teams data **/

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
  FREE_CUSTOM(team->users, twitch_channel_list_free)
  free(team);
}

GENERIC_LIST(team)

