#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/strings/strings.h"
#include "utils/network/helix.h"
#include "utils/parser/parser.h"
#include "json/json.h"

#include <ctwitch/helix/data.h>

#define MAX_USERS_COUNT 100

/** URL builders	**/

typedef struct {
	const char *game_id;
	const char *language;
	int users_count;
	const char **users;
	int logins_count;
	const char **logins;
} helix_streams_params;

string_t *helix_streams_url_builder(
	void *params,
	int limit,
	const char *after
) {
	char buffer[128];
	bool is_first_param = true;

	helix_streams_params *sparams = (helix_streams_params *)params;

	// Construct the link.
	string_t *url = string_init_with_value("https://api.twitch.tv/helix/streams");

	if (sparams->game_id > 0) {
		string_append_format(url, "?game_id=%s", sparams->game_id);
		is_first_param = false;
	}

	if (sparams->language != NULL) {
		string_append_format(
			url, "%slanguage=%s",
			is_first_param ? "?" : "&", sparams->language
		);
		is_first_param = false;
	}

	for (
		int idx = 0;
		idx < sparams->logins_count && idx < MAX_USERS_COUNT;
		idx++
	) {
		string_append_format(
			url, "%suser_login=%s",
			is_first_param ? "?" : "&", sparams->logins[idx]
		);
		is_first_param = false;
	}

	for (
		int idx = 0;
		idx < sparams->users_count && idx < MAX_USERS_COUNT;
		idx++
	) {
		string_append_format(url, "%suser_id=%s",
			is_first_param ? "?" : "&", sparams->users[idx]
		);
		is_first_param = false;
	}

	helix_append_cursor_params(url, limit, after, is_first_param);

	return url;
}

twitch_helix_stream_list *twitch_helix_get_streams(
	const char *client_id,
	const char *auth,
	const char *game_id,
	const char *language,
	int users_count,
	const char **users,
	int logins_count,
	const char **logins,
	int limit,
	const char *after,
	int *total,
	char **next
) {
	helix_streams_params params = {
		.game_id = game_id,
		.language = language,
		.logins_count = logins_count,
		.logins = logins,
		.users_count = users_count,
		.users = users
	};

	twitch_helix_stream_list *list = twitch_helix_stream_list_alloc();
	list->items = (twitch_helix_stream **)helix_get_page(
		client_id,
		auth,
		&helix_streams_url_builder,
		(void *)&params,
		limit,
		after,
		&parse_helix_stream,
		&list->count,
		next,
		total
	);
	return list;
}

twitch_helix_stream_list *twitch_helix_get_all_streams(
	const char *client_id,
	const char *auth,
	const char *game_id,
	const char *language,
	int users_count,
	const char **users,
	int logins_count,
	const char **logins
) {
	helix_streams_params params = {
		.game_id = game_id,
		.language = language,
		.logins_count = logins_count,
		.logins = logins,
		.users_count = users_count,
		.users = users
	};

	twitch_helix_stream_list *streams = twitch_helix_stream_list_alloc();
	streams->items = (twitch_helix_stream **)get_all_helix_pages(
		client_id,
		auth,
		&helix_streams_url_builder,
		(void *)&params,
		&parse_helix_stream,
		0,
		&streams->count
	);

	return streams;
}
