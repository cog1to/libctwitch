#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "data.h"

/** URL builders  **/

typedef struct {
  const char *user_id;
  const char *direction;
  const char *sortby;
} follows_params;

string_t *user_follows_url_builder(void *params, int limit, int offset) {
  char buffer[128];
  bool is_first_param = true;

  follows_params *fparams = (follows_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)fparams->user_id, strlen(fparams->user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);

  // Append direction.
  if (fparams->direction != NULL) {
    sprintf(buffer, "?direction=%s", fparams->direction);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  // Append sorting order.
  if (fparams->sortby != NULL) {
    sprintf(buffer, "%ssortby=%s", is_first_param ? "?" : "&", fparams->sortby);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  append_paging_params(url, limit, offset, is_first_param);

  return url;
}

/** API **/

twitch_user *twitch_v5_get_user(const char *client_id, const char *id) {
  twitch_user *user = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)id, strlen(id), url);

  // Get JSON.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  if (value->type != json_object) {
    fprintf(stderr, "Wrong JSON type of returned value.");
    exit(EXIT_FAILURE);
  }

  user = parse_user(value);
  json_value_free(value);

  return user;
}

twitch_user *twitch_v5_get_user_by_username(const char *client_id, const char *username) {
  // Output value.
  twitch_user *user = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users?login=");
  string_append((void *)username, strlen(username), url);

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      json_value *user_object = users_value->u.array.values[0];
      user = parse_user(user_object);
    }
  }

  json_value_free(value);
  return user;
}

twitch_user **twitch_v5_get_users(const char *client_id, int usernames_count, const char **usernames, int *total) {
  char buffer[2048];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users");

  // Append usernames.
  if (usernames_count > 0) {
    string_t *usernames_string = string_joined(usernames_count, usernames, ",");
    sprintf(buffer, "?login=%s", usernames_string->ptr);
    string_append((void *)buffer, strlen(buffer), url);
    string_free(usernames_string);
  }

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  twitch_user **users = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      users = (twitch_user **)parse_json_array(users_value, total, &parse_user);
    }
  }

  json_value_free(value);
  return users;
}

twitch_follow *twitch_v5_check_user_follow(const char *client_id, const char *user_id, const char *channel_id) {
  // Output value.
  twitch_follow *follow = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)user_id, strlen(user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);
  string_append((void *)channel_id, strlen(channel_id), url);

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  if (value == NULL) {
    return NULL;
  }

  follow = parse_follow(value);
  json_value_free(value);

  return follow;
}

twitch_follow **twitch_v5_get_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int limit, int offset, int *size, int *total) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_page(client_id, &user_follows_url_builder, (void *)&params, limit, offset, "follows", &parse_follow, size, total);
  return follows;
}

twitch_follow** twitch_v5_get_all_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int *size) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_all_pages(client_id, &user_follows_url_builder, (void *)&params, "follows", &parse_follow, false, size);
  return follows;
}
