#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils/strings/strings.h"
#include "utils/network/helix.h"
#include "utils/parser/parser.h"
#include "json/json.h"

#include <ctwitch/helix/data.h>

/** URL builders **/

string_t *helix_top_games_url_builder(
	void *params,
	int limit,
	const char *after
) {
	string_t *url = string_init_with_value("https://api.twitch.tv/helix/games/top");

	// Top Games endpoint doesn't have any additional parameters, so we just add
	// paging data.
	helix_append_cursor_params(url, limit, after, true);

	return url;
}

/** API **/

twitch_helix_game_list *twitch_helix_get_top_games(
	const char *client_id,
	const char *token,
	twitch_error *error,
	int first,
	const char *after,
	char **next
) {
	twitch_helix_game_list *list = twitch_helix_game_list_alloc();

	list->items = (twitch_helix_game **)helix_get_page(
		client_id,
		token,
		error,
		&helix_top_games_url_builder,
		NULL,
		first,
		after,
		&parse_helix_game,
		&list->count,
		next,
		NULL
	);

	return list;
}

twitch_helix_game_list *twitch_helix_get_all_top_games(
	const char *client_id,
	const char *token,
	twitch_error *error,
	int limit
) {
	twitch_helix_game_list *list = twitch_helix_game_list_alloc();

	list->items = (twitch_helix_game **)get_all_helix_pages(
		client_id,
		token,
		error,
		&helix_top_games_url_builder,
		NULL,
		&parse_helix_game,
		limit,
		&list->count
	);

	return list;
}

