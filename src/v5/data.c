#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/datagen.h"
#include "v5/data.h"
#include "utils/array_utils.h"
#include "utils/data_utils.h"

/** User data **/

twitch_v5_user *twitch_v5_user_alloc() {
  GENERIC_ALLOC(twitch_v5_user)
}

void twitch_v5_user_free(twitch_v5_user *user) {
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

twitch_v5_channel *twitch_v5_channel_alloc() {
  GENERIC_ALLOC(twitch_v5_channel)
}

void twitch_v5_channel_free(twitch_v5_channel *channel) {
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

twitch_v5_follow *twitch_v5_follow_alloc() {
  GENERIC_ALLOC(twitch_v5_follow)
}

void twitch_v5_follow_free(twitch_v5_follow *follow) {
  FREE(follow->created_at)
  FREE_CUSTOM(follow->channel, twitch_v5_channel_free);
  free(follow);
}

GENERIC_LIST(follow)

/** Streams **/

twitch_v5_stream *twitch_v5_stream_alloc() {
  GENERIC_ALLOC(twitch_v5_stream)
}

void twitch_v5_stream_free(twitch_v5_stream *stream) {
  FREE(stream->created_at)
  FREE(stream->game)
  FREE_CUSTOM(stream->channel, twitch_v5_channel_free);
  FREE_CUSTOM(stream->preview, twitch_v5_art_free);
  free(stream);
}

GENERIC_LIST(stream)

twitch_v5_summary *twitch_v5_summary_alloc() {
  GENERIC_ALLOC(twitch_v5_summary)
}

void twitch_v5_summary_free(twitch_v5_summary *summary) {
  free(summary);
}

twitch_v5_featured_stream *twitch_v5_featured_stream_alloc() {
  GENERIC_ALLOC(twitch_v5_featured_stream)
}

void twitch_v5_featured_stream_free(twitch_v5_featured_stream *stream) {
  FREE(stream->image)
  FREE(stream->text)
  FREE(stream->title)
  FREE_CUSTOM(stream->stream, twitch_v5_stream_free);
  free(stream);
}

GENERIC_LIST(featured_stream)

/** Art data **/

twitch_v5_art *twitch_v5_art_alloc() {
  GENERIC_ALLOC(twitch_v5_art)
}

void twitch_v5_art_free(twitch_v5_art *art) {
  FREE(art->large)
  FREE(art->medium)
  FREE(art->small)
  FREE(art->template)
  free(art);
}

/** Game data  **/

twitch_v5_game *twitch_v5_game_alloc() {
  GENERIC_ALLOC(twitch_v5_game)
}

void twitch_v5_game_free(twitch_v5_game *game) {
  FREE_CUSTOM(game->box, twitch_v5_art_free)
  FREE_CUSTOM(game->logo, twitch_v5_art_free)
  FREE(game->name)
  FREE(game->localized_name)
  FREE(game->locale)
  free(game);
}

GENERIC_LIST(game)

/** Top games data **/

twitch_v5_top_game *twitch_v5_top_game_alloc() {
  GENERIC_ALLOC(twitch_v5_top_game)
}

void twitch_v5_top_game_free(twitch_v5_top_game *game) {
  FREE_CUSTOM(game->game, twitch_v5_game_free)
  free(game);
}

GENERIC_LIST(top_game)

/** Channel followers data **/

twitch_v5_follower *twitch_v5_follower_alloc() {
  GENERIC_ALLOC(twitch_v5_follower)
}

void twitch_v5_follower_free(twitch_v5_follower *follower) {
  FREE_CUSTOM(follower->user, twitch_v5_user_free)
  FREE(follower->created_at)
  free(follower);
}

GENERIC_LIST(follower)

/** Communities data **/

twitch_v5_community *twitch_v5_community_alloc() {
  GENERIC_ALLOC(twitch_v5_community)
}

void twitch_v5_community_free(twitch_v5_community *community) {
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

twitch_v5_resolutions *twitch_v5_resolutions_alloc() {
  GENERIC_ALLOC(twitch_v5_resolutions)
}

void twitch_v5_resolutions_free(twitch_v5_resolutions *resolutions) {
  FREE(resolutions->chunked)
  FREE(resolutions->high)
  FREE(resolutions->medium)
  FREE(resolutions->low)
  FREE(resolutions->mobile)
  free(resolutions);
}

twitch_v5_fps *twitch_v5_fps_alloc() {
  GENERIC_ALLOC(twitch_v5_fps)
}

void twitch_v5_fps_free(twitch_v5_fps *fps) {
  free(fps);
}

twitch_v5_channel_ref *twitch_v5_channel_ref_alloc() {
  GENERIC_ALLOC(twitch_v5_channel_ref)
}

void twitch_v5_channel_ref_free(twitch_v5_channel_ref *ref) {
  FREE(ref->name)
  FREE(ref->display_name)
  free(ref);
}

twitch_v5_thumbnail *twitch_v5_thumbnail_alloc() {
  GENERIC_ALLOC(twitch_v5_thumbnail)
}

void twitch_v5_thumbnail_free(twitch_v5_thumbnail *thumbnail) {
  FREE(thumbnail->url)
  FREE(thumbnail->type)
  free(thumbnail);
}

GENERIC_LIST(thumbnail)

twitch_v5_video_thumbnails *twitch_v5_video_thumbnails_alloc() {
  GENERIC_ALLOC(twitch_v5_video_thumbnails)
}

void twitch_v5_video_thumbnails_free(twitch_v5_video_thumbnails *thumbnails) {
  FREE_CUSTOM(thumbnails->large, twitch_v5_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->medium, twitch_v5_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->small, twitch_v5_thumbnail_list_free)
  FREE_CUSTOM(thumbnails->template, twitch_v5_thumbnail_list_free)
  free(thumbnails);
}

twitch_v5_video *twitch_v5_video_alloc() {
  GENERIC_ALLOC(twitch_v5_video)
}

void twitch_v5_video_free(twitch_v5_video *video) {
  FREE(video->id)
  FREE(video->broadcast_type)
  FREE_CUSTOM(video->channel, twitch_v5_channel_ref_free)
  FREE(video->created_at)
  FREE(video->description)
  FREE(video->description_html)
  FREE_CUSTOM(video->fps, twitch_v5_fps_free)
  FREE(video->game)
  FREE(video->language)
  FREE_CUSTOM(video->preview, twitch_v5_art_free)
  FREE(video->published_at)
  FREE_CUSTOM(video->resolutions, twitch_v5_resolutions_free)
  FREE(video->status)
  FREE(video->tag_list)
  FREE(video->title)
  FREE(video->url)
  FREE_CUSTOM(video->thumbnails, twitch_v5_video_thumbnails_free)
  FREE(video->viewable)
  FREE(video->viewable_at)
  free(video);
}

GENERIC_LIST(video)

/** Teams data **/

twitch_v5_team *twitch_v5_team_alloc() {
  GENERIC_ALLOC(twitch_v5_team)
}

void twitch_v5_team_free(twitch_v5_team *team) {
  FREE(team->background)
  FREE(team->banner)
  FREE(team->created_at)
  FREE(team->display_name)
  FREE(team->info)
  FREE(team->logo)
  FREE(team->name)
  FREE(team->updated_at)
  FREE_CUSTOM(team->users, twitch_v5_channel_list_free)
  free(team);
}

GENERIC_LIST(team)

