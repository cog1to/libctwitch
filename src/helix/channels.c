#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_helix_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#include <ctwitch/helix/data.h>

/** Channel followers **/

typedef struct {
	const char *user_id;
	const char *broadcaster_id;
} helix_channel_followers_params;

string_t *helix_channel_followers_url_builder(
	void *params,
	int limit,
	const char *after
) {
	helix_channel_followers_params *fparams =
		(helix_channel_followers_params *)params;

	// construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/channels/followers"
	);
	string_append_format(url, "?broadcaster_id=%s", fparams->broadcaster_id);

	// append user_id.
	if (fparams->user_id != NULL) {
		string_append_format(url, "&user_id=%s", fparams->user_id);
	}

	helix_append_cursor_params(url, limit, after, true);

	return url;
}

twitch_helix_follower_list *twitch_helix_get_channel_followers(
	const char *client_id,
	const char *token,
	const char *channel_id,
	const char *user_id,
	int first,
	const char *after,
	int *total,
	char **next
) {
	helix_channel_followers_params params = {
		.broadcaster_id = channel_id,
		.user_id = user_id,
	};

	twitch_helix_follower_list *list = twitch_helix_follower_list_alloc();

	list->items = (twitch_helix_follower **)helix_get_page(
		client_id,
		token,
		&helix_channel_followers_url_builder,
		(void *)&params,
		first,
		after,
		&parse_helix_follower,
		&list->count,
		next,
		total
	);

	return list;
}

twitch_helix_follower_list *twitch_helix_get_all_channel_followers(
	const char *client_id,
	const char *token,
	const char *channel_id,
	const char *user_id,
	int limit
) {
	helix_channel_followers_params params = {
		.broadcaster_id = channel_id,
		.user_id = user_id,
	};

	twitch_helix_follower_list *list = twitch_helix_follower_list_alloc();

	list->items = (twitch_helix_follower **)get_all_helix_pages(
		client_id,
		token,
		&helix_channel_followers_url_builder,
		(void *)&params,
		&parse_helix_follower,
		limit,
		&list->count
	);

	return list;
}

/** Channel teams **/

string_t *helix_channel_teams_url_builder(
	void *params,
	int limit,
	const char *after
) {
	char *channel_id = (char *)params;

	// construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/teams/channel"
	);
	string_append_format(url, "?broadcaster_id=%s", channel_id);

	return url;
}

twitch_helix_team_list *twitch_helix_get_channel_teams(
	const char *client_id,
	const char *token,
	const char *channel_id
) {
	twitch_helix_team_list *list = twitch_helix_team_list_alloc();

	list->items = (twitch_helix_team **)helix_get_page(
		client_id,
		token,
		&helix_channel_teams_url_builder,
		(void *)channel_id,
		0,
		NULL,
		&parse_helix_team,
		&list->count,
		NULL,
		NULL
	);

	return list;
}
