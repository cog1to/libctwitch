#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/network_helix_utils.h"
#include "utils/string_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#define MAX_PAGE_SIZE 100
#define DEFAULT_PAGE_SIZE 20

/** Helpers **/

int min_int(int a, int b) {
	if (a < b) { return a; } else { return b; }
}

/** cURL helpers **/

extern size_t twitch_writefunc(
	void *ptr,
	size_t size,
	size_t nmemb,
	struct string *s
);
extern const char *url_encode(const char *string);

CURLcode twitch_helix_get(
	const char *client_id,
	const char *auth,
	const char *url,
	string_t *output
) {
	// Initialize curl.
	CURL *curl;
	curl = curl_easy_init();

	// Headers list.
	struct curl_slist *headers = NULL;

	// Construct client ID header.
	string_t *client_id_header = string_init_with_value("Client-ID: ");
	string_append((void *)client_id, strlen(client_id), client_id_header);
	headers = curl_slist_append(headers, client_id_header->ptr);

	// Construct Authorization header.
	string_t *auth_header = string_init_with_value("Authorization: Bearer ");
	string_append((void *)auth, strlen(auth), auth_header);
	headers = curl_slist_append(headers, auth_header->ptr);

	// Set headers.
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Set URL.
	curl_easy_setopt(curl, CURLOPT_URL, url);

	// Setup output buffer.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, twitch_writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

	// Perform curl operation.
	CURLcode code = curl_easy_perform(curl);

	// Cleanup.
	curl_slist_free_all(headers);
	string_free(client_id_header);
	curl_easy_cleanup(curl);

	return code;
}

json_value *twitch_helix_get_json(
	const char *client_id,
	const char *auth,
	const char *url
) {
	// Get the output.
	string_t *output = string_init();
	CURLcode code = twitch_helix_get(client_id, auth, url, output);

	// Check return code.
	if (code == CURLE_HTTP_RETURNED_ERROR) {
		fprintf(stderr, "Error at url '%s'\n", url);
		string_free(output);
		return NULL;
	}

	// Dump
	printf("%s\n", output->ptr);

	// Parse.
	json_value *value = json_parse(output->ptr, output->len);
	string_free(output);

	if (value == NULL) {
		fprintf(stderr, "Failed to parse JSON.\n");
	}

	return value;
}

/** Helpers **/

void helix_append_cursor_params(
	string_t *url,
	int limit,
	const char *after,
	bool is_first_param
) {
	bool is_first_param_updated = is_first_param;

	if (limit > 0) {
		string_append_format(
			url, "%sfirst=%i",
			is_first_param_updated ? "?" : "&",
			min_int(limit, MAX_PAGE_SIZE)
		);
		is_first_param_updated = false;
	}

	if (after != NULL) {
		string_append_format(
			url, "%safter=%s",
			is_first_param_updated ? "?" : "&",
			after
		);
	}
}

void **helix_get_page(
	const char *client_id,
	const char *auth,
	helix_page_url_builder builder,
	void *params,
	int limit,
	const char *after,
	parser_func parser,
	int *size,
	char **next,
	int *total
) {
	string_t *url = builder(params, limit, after);
	printf("URL: %s", url->ptr);
	json_value *value = twitch_helix_get_json(client_id, auth, url->ptr);
	string_free(url);

	if (value == NULL) {
		*size = 0;
		return NULL;
	}

	// Extract the relevant fields.
	void **elements = NULL;

	int length = value->u.object.length;
	for (int x = 0; x < length; x++) {
		if (strcmp(value->u.object.values[x].name, "data") == 0) {
			json_value *elements_value = value->u.object.values[x].value;
			int elements_length = elements_value->u.array.length;
			if (elements_length == 0) {
				*size = 0;
				break;
			}

			elements = parse_json_array(elements_value, size, parser);
		} else if (strcmp(value->u.object.values[x].name, "pagination") == 0) {
			json_value *pagination = value->u.object.values[x].value;
			int pagination_length = pagination->u.object.length;
			for (int y = 0; y < pagination_length; y++) {
				if (strcmp(pagination->u.object.values[y].name, "cursor") == 0 && next != NULL) {
					(*next) = immutable_string_copy(pagination->u.object.values[y].value->u.string.ptr);
				}
			}
		} else if (strcmp(value->u.object.values[x].name, "total") == 0 && total != NULL) {
			*total = value->u.object.values[x].value->u.integer;
		}
	}

	json_value_free(value);
	return elements;
}

void **get_all_helix_pages(
	const char *client_id,
	const char *auth,
	helix_page_url_builder builder,
	void *params,
	parser_func parser,
	int limit,
	int *size
) {
	const int PAGE_SIZE = DEFAULT_PAGE_SIZE;

	int count = 0;
	int total = 0;
	int reported_total = 0;
	void **elements = NULL;
	char *cursor = NULL;
	char *next_cursor = NULL;

	do {
		void **page = helix_get_page(
			client_id,
			auth,
			builder,
			params,
			min_int(PAGE_SIZE, (limit > 0) ? (limit - total) : 0),
			cursor,
			parser,
			&count,
			&next_cursor,
			&reported_total
		);

		// Don't do anything if there are 0 items returned. It should mean we're at
		// the end of the list.
		if (count == 0 && next_cursor == NULL) {
			break;
		}

		// (Re)allocate memory to store next page.
		if (total == 0) {
			elements = malloc(sizeof(void *) * count);
		} else {
			elements = realloc(elements, sizeof(void *) * (total + count));
		}

		if (elements == NULL) {
			fprintf(stderr, "Failed to allocate memory for next page.\n");
			exit(EXIT_FAILURE);
		}

		// Copy page's content to the overall storage.
		memcpy(&elements[total], page, sizeof(void *) * count);

		// Update total count.
		total += count;

		// Free current page data.
		free(page);

		// Update cursor, if needed.
		if (next_cursor != NULL) {
			if (cursor != NULL) {
				free(cursor);
			}
			cursor = immutable_string_copy(next_cursor);
			free(next_cursor);
			next_cursor = NULL;
		}
	} while (count > 0 && total < reported_total && (limit == 0 || total < limit));

	// Free the cursor memory.
	if (cursor != NULL) {
		free(cursor);
	}

	// Return the whole list.
	*size = total;
	return elements;
}
