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
  const char *channel_id;
  const char *direction;
} channel_followers_params;

string_t *channel_followers_url_builder(void *params, int limit, int offset, const char *cursor) {
  char buffer[128];
  bool is_first_param = true;

  channel_followers_params *cfparams = (channel_followers_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/channels/");
  string_append((void *)cfparams->channel_id, strlen(cfparams->channel_id), url);
  string_append((void *)"/follows", strlen("/follows"), url);

  // Append direction.
  if (cfparams->direction != NULL) {
    sprintf(buffer, "?direction=%s", cfparams->direction);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  append_paging_params(url, limit, offset, is_first_param);
  append_cursor(url, cursor, is_first_param);

  return url;
}

/** API **/

twitch_follower **twitch_v5_get_channel_followers(const char *client_id, const char *channel_id, int limit, int offset, const char *cursor, const char *direction, int *size, int *total, char **next_cursor) {
  channel_followers_params params = {
    .channel_id = channel_id,
    .direction = direction,
  };

  twitch_follower **followers = (twitch_follower **)get_page(client_id, &channel_followers_url_builder, (void *)&params, limit, offset, cursor, "follows", &parse_follower, size, total, next_cursor);
  return followers;
}

twitch_follower **twitch_v5_get_all_channel_followers(const char *client_id, const char *channel_id, const char *direction, int *size) {
  channel_followers_params params = {
    .channel_id = channel_id,
    .direction = direction,
  };

  twitch_follower **followers = (twitch_follower **)get_all_pages(client_id, &channel_followers_url_builder, (void *)&params, "follows", &parse_follower, false, size);
  return followers;
}

