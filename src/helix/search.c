#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "utils/string_utils.h"
#include "utils/network_helix_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#include <ctwitch/helix/data.h>

/** Categories **/

string_t *helix_categories_url_builder(
	void *params,
	int limit,
	const char *after
) {
	char *query = (char *)params;

	// Construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/search/categories"
	);

	char *urlencoded_query = url_encode(query);
	string_append((void *)"?query=", strlen("?query="), url);
	string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

	helix_append_cursor_params(url, limit, after, false);
	free(urlencoded_query);

	printf("%s\n", url->ptr);

	return url;
}

twitch_helix_category_list *twitch_helix_get_categories(
	const char *client_id,
	const char *token,
	const char *query,
	int first,
	const char *after,
	char **next
) {
	twitch_helix_category_list *list = twitch_helix_category_list_alloc();

	list->items = (twitch_helix_category **)helix_get_page(
		client_id,
		token,
		&helix_categories_url_builder,
		(void *)query,
		first,
		after,
		&parse_helix_category,
		&list->count,
		next,
		NULL
	);

	return list;
}

twitch_helix_category_list *twitch_helix_get_all_categories(
	const char *client_id,
	const char *token,
	const char *query,
	int limit
) {
	twitch_helix_category_list *list = twitch_helix_category_list_alloc();

	list->items = (twitch_helix_category **)get_all_helix_pages(
		client_id,
		token,
		&helix_categories_url_builder,
		(void *)query,
		&parse_helix_category,
		limit,
		&list->count
	);

	return list;
}

/** Channels **/

typedef struct {
	const char *query;
	int live_only;
	int first;
} twitch_channel_search_params;

string_t *helix_channel_search_url_builder(
	void *params,
	int limit,
	const char *after
) {
	twitch_channel_search_params *sparams =
		(twitch_channel_search_params *)params;

	// Construct the link.
	string_t *url = string_init_with_value(
		"https://api.twitch.tv/helix/search/channels"
	);

	char *urlencoded_query = url_encode(sparams->query);
	string_append((void *)"?query=", strlen("?query="), url);
	string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

	if (sparams->live_only) {
		string_append((void *)"&live_only=true", strlen("&live_only=true"), url);
	}

	helix_append_cursor_params(url, limit, after, false);
	free(urlencoded_query);
	return url;
}

twitch_helix_channel_search_item_list *twitch_helix_search_channels(
	const char *client_id,
	const char *token,
	const char *query,
	int live_only,
	int first,
	const char *after,
	char **next
) {
	twitch_channel_search_params params = {
		.query = query,
		.live_only = live_only
	};

	twitch_helix_channel_search_item_list *list =
		twitch_helix_channel_search_item_list_alloc();

	list->items = (twitch_helix_channel_search_item **)helix_get_page(
		client_id,
		token,
		&helix_channel_search_url_builder,
		(void *)&params,
		first,
		after,
		&parse_helix_channel_search_item,
		&list->count,
		next,
		NULL
	);

	return list;
}

twitch_helix_channel_search_item_list *twitch_helix_search_all_channels(
	const char *client_id,
	const char *token,
	const char *query,
	int live_only,
	int limit
) {
	twitch_channel_search_params params = {
		.query = query,
		.live_only = live_only
	};

	twitch_helix_channel_search_item_list *list =
		twitch_helix_channel_search_item_list_alloc();

	list->items = (twitch_helix_channel_search_item **)get_all_helix_pages(
		client_id,
		token,
		&helix_channel_search_url_builder,
		(void *)&params,
		&parse_helix_channel_search_item,
		limit,
		&list->count
	);

	return list;
}

