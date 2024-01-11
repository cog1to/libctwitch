#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/datagen.h"
#include "utils/array_utils.h"
#include "utils/data_utils.h"

#include <ctwitch/helix/data.h>

/** User data */

twitch_helix_user *twitch_helix_user_alloc() {
	GENERIC_ALLOC(twitch_helix_user);
}

void twitch_helix_user_free(twitch_helix_user *user) {
	FREE(user->login);
	FREE(user->display_name);
	FREE(user->type);
	FREE(user->broadcaster_type);
	FREE(user->description);
	FREE(user->profile_image_url);
	FREE(user->offline_image_url);
	FREE(user->created_at);
	free(user);
}

GENERIC_HELIX_LIST(user)

/** Channel follows **/

twitch_helix_channel_follow *twitch_helix_channel_follow_alloc() {
	GENERIC_ALLOC(twitch_helix_channel_follow)
}

void twitch_helix_channel_follow_free(twitch_helix_channel_follow *follow) {
	FREE(follow->broadcaster_login);
	FREE(follow->broadcaster_name);
	FREE(follow->followed_at);
	free(follow);
}

GENERIC_HELIX_LIST(channel_follow)

/** Stream **/

twitch_helix_stream *twitch_helix_stream_alloc() {
	GENERIC_ALLOC(twitch_helix_stream)
}

void twitch_helix_stream_free(twitch_helix_stream *stream) {
	FREE(stream->user_name);
	FREE(stream->game_name);
	FREE(stream->type);
	FREE(stream->title);
	FREE(stream->started_at);
	FREE(stream->language);
	FREE(stream->thumbnail_url);
	free(stream);
}

GENERIC_HELIX_LIST(stream)

/** Games **/

twitch_helix_game *twitch_helix_game_alloc() {
	GENERIC_ALLOC(twitch_helix_game)
}

void twitch_helix_game_free(twitch_helix_game *game) {
	FREE(game->id)
	FREE(game->igdb_id)
	FREE(game->name)
	FREE(game->box_art_url)
	free(game);
}

GENERIC_HELIX_LIST(game)

/** Teams **/

twitch_helix_team_member *twitch_helix_team_member_alloc() {
	GENERIC_ALLOC(twitch_helix_team_member)
}

void twitch_helix_team_member_free(twitch_helix_team_member *user) {
	FREE(user->id)
	FREE(user->name)
	FREE(user->login)
	free(user);
}

GENERIC_HELIX_LIST(team_member)

twitch_helix_team *twitch_helix_team_alloc() {
	GENERIC_ALLOC(twitch_helix_team)
}

void twitch_helix_team_free(twitch_helix_team *team) {
	FREE(team->background)
	FREE(team->banner)
	FREE(team->created_at)
	FREE(team->updated_at)
	FREE(team->info)
	FREE(team->thumbnail)
	FREE(team->name)
	FREE(team->display_name)
	FREE(team->id)
	FREE_CUSTOM(team->users, twitch_helix_team_member_list_free)
	free(team);
}

GENERIC_HELIX_LIST(team)

/** Followers **/

twitch_helix_follower *twitch_helix_follower_alloc() {
	GENERIC_ALLOC(twitch_helix_follower)
}

void twitch_helix_follower_free(twitch_helix_follower *follower) {
	FREE(follower->user_id)
	FREE(follower->user_name)
	FREE(follower->user_login)
	FREE(follower->followed_at)
	free(follower);
}

GENERIC_HELIX_LIST(follower)

/** Video **/

twitch_helix_segment *twitch_helix_segment_alloc() {
	GENERIC_ALLOC(twitch_helix_segment)
}

void twitch_helix_segment_free(twitch_helix_segment *segment) {
	free(segment);
}

GENERIC_HELIX_LIST(segment)

twitch_helix_video *twitch_helix_video_alloc() {
	GENERIC_ALLOC(twitch_helix_video)
}

void twitch_helix_video_free(twitch_helix_video *video) {
	FREE(video->id)
	FREE(video->stream_id)
	FREE(video->user_id)
	FREE(video->user_login)
	FREE(video->user_name)
	FREE(video->title)
	FREE(video->description)
	FREE(video->created_at)
	FREE(video->published_at)
	FREE(video->url)
	FREE(video->thumbnail_url)
	FREE(video->viewable)
	FREE(video->language)
	FREE(video->type)
	FREE(video->duration)
	FREE_CUSTOM(video->muted_segments, twitch_helix_segment_list_free)
	free(video);
}

GENERIC_HELIX_LIST(video)

