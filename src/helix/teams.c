#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/strings/strings.h"
#include "utils/network/helix.h"
#include "utils/parser/parser.h"
#include "json/json.h"

#include <ctwitch/common.h>
#include <ctwitch/helix/data.h>

/** URL builders	**/

typedef struct {
	const char *name;
	const char *id;
} helix_teams_params;

string_t *helix_team_url_builder(void *params) {
	string_t *url = string_init_with_value("https://api.twitch.tv/helix/teams");

	helix_teams_params *sparams = (helix_teams_params *)params;

	if (sparams->id) {
		string_append_format(url, "?id=%s", sparams->id);
	} else if (sparams->name) {
		string_append_format(url, "?name=%s", sparams->name);
	}

	return url;
}

/** API **/

twitch_helix_team *twitch_helix_get_team(
	const char *client_id,
	const char *bearer,
	twitch_error *error,
	const char *name,
	const char *id
) {
	helix_teams_params params = {
		.name = name,
		.id = id
	};

	string_t *url = helix_team_url_builder(&params);
	json_value *value = twitch_helix_get_json(client_id, bearer, error, url->ptr);
	string_free(url);

	void *team = parse_helix_team(value);
	json_value_free(value);
	return (twitch_helix_team *)team;
}

