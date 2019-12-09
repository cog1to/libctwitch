#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "v5/data.h"

/** URL builders **/

string_t *top_games_url_builder(void *params, int limit, int offset) {
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/games/top");

  // Top Games endpoint doesn't have any additional parameters, so we just add paging data.
  append_paging_params(url, limit, offset, true);

  return url;
}

/** API **/

twitch_v5_top_game_list *twitch_v5_get_top_games(const char *client_id, int limit, int offset, int *total) {
  twitch_v5_top_game_list *list = twitch_v5_top_game_list_alloc();
  list->items = (twitch_v5_top_game **)get_page(client_id, &top_games_url_builder, NULL, limit, offset, "top", &parse_top_game, &list->count, total);

  return list;
}

twitch_v5_top_game_list *twitch_v5_get_all_top_games(const char *client_id) {
  twitch_v5_top_game_list *list = twitch_v5_top_game_list_alloc();

  list->items = (twitch_v5_top_game **)get_all_pages(client_id, &top_games_url_builder, NULL, "games", &parse_top_game, false, &list->count);

  return list;
}

