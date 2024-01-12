#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/datagen.h"
#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#include <ctwitch/auth.h>

/** Utils **/

twitch_string_list *twitch_string_list_alloc() {
	GENERIC_ALLOC(twitch_string_list)
}

void twitch_string_list_free(twitch_string_list *list) {
	if (list == NULL) {
		return;
	}

	if (list->items != NULL) {
		for (int idx = 0; idx < list->count; idx++) {
			free(list->items[idx]);
		}	
		free(list->items);
	}

	free(list);
}

/** Token **/

twitch_app_access_token *twitch_app_access_token_alloc() {
	GENERIC_ALLOC(twitch_app_access_token)
}

void twitch_app_access_token_free(twitch_app_access_token *token) {
	FREE(token->token)
	FREE(token->token_type)
	free(token);
}

twitch_user_access_token *twitch_user_access_token_alloc() {
	GENERIC_ALLOC(twitch_user_access_token)
}

void twitch_user_access_token_free(twitch_user_access_token *token) {
	FREE(token->access_token)
	FREE(token->refresh_token)
	FREE(token->token_type)
	if (token->scopes.count > 0) {
		for (int idx = 0; idx < token->scopes.count; idx++) {
			free(token->scopes.items[idx]);
		}
	}
	free(token);
}

/** API **/

twitch_app_access_token *twitch_get_app_access_token(
	const char *client_id,
	const char *client_secret
) {
	twitch_app_access_token *token = NULL;
	const char *grant_type = "&grant_type=client_credentials";

	// Construct the link.
	string_t *url = string_init_with_value("https://id.twitch.tv/oauth2/token");
	string_append_format(url, "?client_id=%s", client_id);
	string_append_format(url, "&client_secret=%s", client_secret);
	string_append(grant_type, strlen(grant_type), url);

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

twitch_user_access_token *twitch_get_user_access_token(
	const char *client_id,
	const char *client_secret,
	const char *code,
	const char *redirect_uri
) {
	twitch_user_access_token *token = NULL;
	const char *grant_type = "&grant_type=authorization_code";

	// Construct the link.
	string_t *url = string_init_with_value("https://id.twitch.tv/oauth2/token");
	string_append_format(url, "?client_id=%s", client_id);
	string_append_format(url, "&client_secret=%s", client_secret);
	string_append_format(url, "&code=%s", code);
	string_append(grant_type, strlen(grant_type), url);
	string_append_format(url, "&redirect_uri=%s", redirect_uri);

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

	token = parse_user_auth_token(value);
	json_value_free(value);

	return token;
}
