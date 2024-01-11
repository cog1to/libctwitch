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
#include <ctwitch/helix/videos.h>

typedef struct {
	const char *user_id;
	const char *game_id;
	int id_count;
	const char **ids;
	const char *language;
	const char *period;
	const char *sort;
	const char *type;
} helix_videos_params;

string_t *helix_videos_url_builder(
	void *params,
	int limit,
	const char *after
) {
	bool is_first_param = true;
	helix_videos_params *vparams =
		(helix_videos_params *)params;

	// construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/videos"
	);

	if (vparams->id_count > 0 && vparams->ids != NULL) {
		for (int idx = 0; idx < vparams->id_count; idx++) {
			string_append_format(
				url,
				"%sid=%s", is_first_param ? "?" : "&", vparams->ids[idx]
			);
			is_first_param = false;
		}
	}

	if (vparams->user_id != NULL) {
		string_append_format(
			url,
			"%suser_id=%s", is_first_param ? "?" : "&", vparams->user_id
		);
		is_first_param = false;
	}

	if (vparams->game_id != NULL) {
		string_append_format(
			url,
			"%sgame_id=%s", is_first_param ? "?" : "&", vparams->game_id
		);
		is_first_param = false;
	}

	if (vparams->language != NULL) {
		string_append_format(
			url,
			"%slanguage=%s", is_first_param ? "?" : "&", vparams->language
		);
		is_first_param = false;
	}

	if (vparams->period != NULL) {
		string_append_format(
			url,
			"%speriod=%s", is_first_param ? "?" : "&", vparams->period
		);
		is_first_param = false;
	}

	if (vparams->sort != NULL) {
		string_append_format(
			url,
			"%ssort=%s", is_first_param ? "?" : "&", vparams->sort
		);
		is_first_param = false;
	}

	if (vparams->type != NULL) {
		string_append_format(
			url,
			"%stype=%s", is_first_param ? "?" : "&", vparams->type
		);
		is_first_param = false;
	}

	helix_append_cursor_params(url, limit, after, is_first_param);

	return url;
}

twitch_helix_video_list *twitch_helix_get_videos(
	const char *client_id,
	const char *token,
	const char *user_id,
	const char *game_id,
	int id_count,
	const char **ids,
	const char *language,
	const char *period,
	const char *sort,
	const char *type,
	int first,
	const char *after,
	char **next
) {
	helix_videos_params params = {
		.user_id = user_id,
		.game_id = game_id,
		.id_count = id_count,
		.ids = ids,
		.language = language,
		.period = period,
		.sort = sort,
		.type = type
	};

	twitch_helix_video_list *list = twitch_helix_video_list_alloc();

	list->items = (twitch_helix_video **)helix_get_page(
		client_id,
		token,
		&helix_videos_url_builder,
		(void *)&params,
		first,
		after,
		&parse_helix_video,
		&list->count,
		next,
		NULL
	);

	return list;
}

twitch_helix_video_list *twitch_helix_get_all_videos(
	const char *client_id,
	const char *token,
	const char *user_id,
	const char *game_id,
	int id_count,
	const char **ids,
	const char *language,
	const char *period,
	const char *sort,
	const char *type,
	int limit
) {
	helix_videos_params params = {
		.user_id = user_id,
		.game_id = game_id,
		.id_count = id_count,
		.ids = ids,
		.language = language,
		.period = period,
		.sort = sort,
		.type = type
	};

	twitch_helix_video_list *list = twitch_helix_video_list_alloc();

	list->items = (twitch_helix_video **)get_all_helix_pages(
		client_id,
		token,
		&helix_videos_url_builder,
		(void *)&params,
		&parse_helix_video,
		limit,
		&list->count
	);

	return list;
}
