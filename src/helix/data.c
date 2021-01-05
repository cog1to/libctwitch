#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/datagen.h"
#include "helix/data.h"
#include "utils/array_utils.h"
#include "utils/data_utils.h"

/** User data **/

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
