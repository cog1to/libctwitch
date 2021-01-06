#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_helix_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "helix/data.h"

/** Constans **/

#define MAX_USERS_COUNT 100

/** URL builders  **/

typedef struct {
  int logins_count;
  const char **logins;
} helix_users_params;

string_t *helix_users_url_builder(void *params, int limit, const char *after) {
  char buffer[128];
  bool is_first_param = true;

  helix_users_params *uparams = (helix_users_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/helix/users");

  for (int idx = 0; idx < uparams->logins_count && idx < MAX_USERS_COUNT; idx++) {
    if (is_first_param) {
      string_append_format(url, "?login=%s", uparams->logins[idx]);
      is_first_param = false;
    } else {
      string_append_format(url, "&login=%s", uparams->logins[idx]);
    }
  }

  helix_append_cursor_params(url, limit, after, is_first_param);

  return url;
}

twitch_helix_user_list *twitch_helix_get_users(
  const char *client_id,
  const char *auth,
  int logins_count,
  const char **logins
) {
  helix_users_params params = {
    .logins_count = logins_count,
    .logins = logins
  };

  twitch_helix_user_list *list = twitch_helix_user_list_alloc();
  list->items = (twitch_helix_user **)helix_get_page(
    client_id,
    auth,
    &helix_users_url_builder,
    (void *)&params,
    0,
    NULL,
    &parse_helix_user,
    &list->count,
    NULL
  );
  return list;
}

twitch_helix_user *twitch_helix_get_user(const char *client_id, const char *auth, const char *login) {
  return NULL;
}

