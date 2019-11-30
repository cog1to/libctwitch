#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "data.h"

/** URL builders **/

string_t *top_games_url_builder(void *params, int limit, int offset) {
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/games/top");

  // Top Games endpoint doesn't have any additional parameters, so we just add paging data.
  append_paging_params(url, limit, offset, true);

  return url;
}

/** API **/

twitch_top_game **twitch_v5_get_top_games(const char *client_id, int limit, int offset, int *size, int *total) {
  twitch_top_game **games = (twitch_top_game **)get_page(client_id, &top_games_url_builder, NULL, limit, offset, "top", &parse_top_game, size, total);
  return games;
}

twitch_top_game** twitch_v5_get_all_top_games(const char *client_id, int *size) {
  twitch_top_game **games = (twitch_top_game **)get_all_pages(client_id, &top_games_url_builder, NULL, "games", &parse_top_game, false, size);
  return games;
}

