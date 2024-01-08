#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#include <ctwitch/helix/data.h>

/** API **/

twitch_helix_auth_token *twitch_helix_get_app_access_token(
	const char *client_id,
	const char *client_secret,
	int scopesv,
	const char **scopesc
) {
	twitch_helix_auth_token *token = NULL;
	const char *grant_type = "&grant_type=client_credentials";
	const char *delimiter = " ";

	// Construct the link.
	string_t *url = string_init_with_value("https://id.twitch.tv/oauth2/token");
	string_append_format(url, "?client_id=%s", client_id);
	string_append_format(url, "&client_secret=%s", client_secret);
	string_append(grant_type, strlen(grant_type), url);

	if (scopesv > 0) {
		string_t *scopes = string_joined(scopesv, scopesc, delimiter);
		string_append_format(url, "&scope=%s", scopes->ptr);
		string_free(scopes);
	}

	// Get JSON.
	json_value *value = twitch_auth_post_json(url->ptr);
	string_free(url);

	if (value == NULL) {
		return NULL;
	}

	if (value->type != json_object) {
		fprintf(stderr, "Wrong JSON type of returned value.");
		exit(EXIT_FAILURE);
	}

	token = parse_auth_token(value);
	json_value_free(value);

	return token;
}

