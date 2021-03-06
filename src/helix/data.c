#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/datagen.h"
#include "helix/data.h"
#include "utils/array_utils.h"
#include "utils/data_utils.h"

/** Token **/

twitch_helix_auth_token *twitch_helix_auth_token_alloc() {
  GENERIC_ALLOC(twitch_helix_auth_token)
}

void twitch_helix_auth_token_free(twitch_helix_auth_token *token) {
  FREE(token->token)
  FREE(token->token_type)
  if (token->scope.count > 0) {
    for (int idx = 0; idx < token->scope.count; idx++) {
      free(token->scope.items[idx]);
    }
  }
  free(token);
}

/** User data */

twitch_helix_user *twitch_helix_user_alloc() {
  GENERIC_ALLOC(twitch_helix_user);
}

void twitch_helix_user_free(twitch_helix_user *user) {
  FREE(user->login);
  FREE(user->display_name);
  FREE(user->type);
  FREE(user->description);
  FREE(user->profile_image_url);
  FREE(user->offline_image_url);
  FREE(user->created_at);
  free(user);
}

GENERIC_HELIX_LIST(user)

/** Follows */

twitch_helix_follow *twitch_helix_follow_alloc() {
  GENERIC_ALLOC(twitch_helix_follow)
}

void twitch_helix_follow_free(twitch_helix_follow *follow) {
  FREE(follow->from_name);
  FREE(follow->to_name);
  FREE(follow->followed_at);
  free(follow);
}

GENERIC_HELIX_LIST(follow)

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

