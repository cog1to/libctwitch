#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "v5/data.h"

/** URL builders  **/

string_t *teams_url_builder(void *params, int limit, int offset) {
  // Teams endpoint doesn't have any additional parameters.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/teams");
  append_paging_params(url, limit, offset, true);

  return url;
}

string_t *team_url_builder(void *params) {
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/teams/");
  string_append((const char *)params, strlen(params), url);
  return url;
}

/** API **/

twitch_v5_team_list *twitch_v5_get_teams(const char *client_id, int limit, int offset) {
  twitch_v5_team_list *list = twitch_v5_team_list_alloc();
  list->items = (twitch_v5_team **)get_page(client_id, &teams_url_builder, NULL, limit, offset, "teams", &parse_team, &list->count, NULL);
  return list;
}

twitch_v5_team_list *twitch_v5_get_all_teams(const char *client_id) {
  twitch_v5_team_list *list = twitch_v5_team_list_alloc();
  list->items = (twitch_v5_team **)get_all_pages(client_id, &teams_url_builder, NULL, "teams", &parse_team, false, &list->count);
  return list;
}

twitch_v5_team *twitch_v5_get_team(const char *client_id, const char *name) {
  string_t *url = team_url_builder((void *)name);
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  void *team = parse_team(value);
  json_value_free(value);
  return (twitch_v5_team *)team;
}

