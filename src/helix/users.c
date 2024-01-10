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

/** Constans **/

#define MAX_USERS_COUNT 100

/** URL builders	**/

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

	for (
		int idx = 0;
		idx < uparams->logins_count && idx < MAX_USERS_COUNT;
		idx++
	) {
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

typedef struct {
	long long user_id;
	long long broadcaster_id;
} helix_channel_follows_params;

string_t *helix_channel_follows_url_builder(
	void *params,
	int limit,
	const char *after
) {
	char buffer[128];
	bool is_first_param = true;

	helix_channel_follows_params *fparams = (helix_channel_follows_params *)params;

	// Construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/channels/followed"
	);

	if (fparams->user_id > 0) {
		string_append_format(
			url,
			"%suser_id=%lld",
			is_first_param ? "?" : "&",
			fparams->user_id
		);
		is_first_param = false;
	}

	if (fparams->broadcaster_id > 0) {
		string_append_format(
			url,
			"%sbroadcaster_id=%lld",
			is_first_param ? "?" : "&",
			fparams->broadcaster_id
		);
		is_first_param = false;
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
		NULL,
		NULL
	);
	return list;
}

twitch_helix_user *twitch_helix_get_user(
	const char *client_id,
	const char *auth,
	const char *login
) {
	const char *usernames[1] = { login };
	twitch_helix_user *output = NULL, *user = NULL;

	twitch_helix_user_list *users = twitch_helix_get_users(
		client_id,
		auth,
		1,
		usernames
	);
	if (users == NULL) {
		return NULL;
	}

	if (users->count == 0) {
		twitch_helix_user_list_free(users);
		return NULL;
	}

	user = users->items[0];

	// Copy the user data.
	output = twitch_helix_user_alloc();
	output->id = user->id;
	output->login = immutable_string_copy(user->login);
	output->display_name = immutable_string_copy(user->display_name);
	output->type = immutable_string_copy(user->type);
	output->description = immutable_string_copy(user->description);
	output->profile_image_url = immutable_string_copy(user->profile_image_url);
	output->offline_image_url = immutable_string_copy(user->offline_image_url);
	output->created_at = immutable_string_copy(user->created_at);

	twitch_helix_user_list_free(users);
	return output;
}

twitch_helix_channel_follow_list *twitch_helix_get_channel_follows(
	const char *client_id,
	const char *auth,
	long long user_id,
	long long broadcaster_id,
	int limit,
	const char *after,
	int *total,
	char **next
) {
	helix_channel_follows_params params = {
		.user_id = user_id,
		.broadcaster_id = broadcaster_id
	};

	twitch_helix_channel_follow_list *list = twitch_helix_channel_follow_list_alloc();
	list->items = (twitch_helix_channel_follow **)helix_get_page(
		client_id,
		auth,
		&helix_channel_follows_url_builder,
		(void *)&params,
		limit,
		after,
		&parse_helix_channel_follow,
		&list->count,
		next,
		total
	);
	return list;
}

twitch_helix_channel_follow_list *twitch_helix_get_all_channel_follows(
	const char *client_id,
	const char *auth,
	long long user_id,
	long long broadcaster_id
) {
	helix_channel_follows_params params = {
		.user_id = user_id,
		.broadcaster_id = broadcaster_id
	};

	twitch_helix_channel_follow_list *follows = twitch_helix_channel_follow_list_alloc();
	follows->items = (twitch_helix_channel_follow **)get_all_helix_pages(
		client_id,
		auth,
		&helix_channel_follows_url_builder,
		(void *)&params,
		&parse_helix_channel_follow,
		0,
		&follows->count
	);

	return follows;
}

