#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <wchar.h>

#include "string_utils.h"
#include "json.h"
#include "data.h"
#include "parser.h"

/** cURL helpers **/

size_t twitch_writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
  size_t append_size = size*nmemb;
  string_append(ptr, append_size, s);
  return append_size;
}

const char *url_encode(const char *string) {
  // Initialize curl.
  CURL *curl;
  curl = curl_easy_init();

  // Perform URL escaping.
  const char *escaped = curl_easy_escape(curl, string, strlen(string));

  curl_easy_cleanup(curl);
  return escaped;
}

CURLcode twitch_v5_get(const char *client_id, const char *url, string_t *output) {
  // Initialize curl.
  CURL *curl;
  curl = curl_easy_init();

  // Headers list.
  struct curl_slist *headers = NULL;

  // Basic headers.
  headers = curl_slist_append(headers, "Accept: application/vnd.twitchtv.v5+json");

  // Construct client ID header.
  string_t *client_id_header = string_init_with_value("Client-ID: ");
  string_append((void *)client_id, strlen(client_id), client_id_header);
  headers = curl_slist_append(headers, client_id_header->ptr);

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

json_value *twitch_v5_get_json(const char *client_id, const char *url) {
  // Get the output.
  string_t *output = string_init();
  CURLcode code = twitch_v5_get(client_id, url, output);

  // Check return code.
  if (code == CURLE_HTTP_RETURNED_ERROR) {
    string_free(output);
    return NULL;
  }
  
  // Parse.
  json_value *value = json_parse(output->ptr, output->len);
  string_free(output);

  if (value == NULL) {
    fprintf(stderr, "Failed to parse JSON.");
    exit(EXIT_FAILURE);
  }

  return value;
}

/** Helpers **/

void append_paging_params(string_t *url, int limit, int offset, bool is_first_param) {
  char buffer[64];
  sprintf(buffer, "%slimit=%i&offset=%i", is_first_param ? "?" : "&", limit, offset);
  string_append((void *)buffer, strlen(buffer), url);
}

typedef string_t *(*page_url_builder)(void *, int limit, int offset);

typedef struct {
  const char *user_id;
  const char *direction;
  const char *sortby;
} follows_params;

string_t *user_follows_url_builder(void *params, int limit, int offset) {
  char buffer[128];
  bool is_first_param = true;

  follows_params *fparams = (follows_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)fparams->user_id, strlen(fparams->user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);

  // Append direction.
  if (fparams->direction != NULL) {
    sprintf(buffer, "?direction=%s", fparams->direction);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  // Append sorting order.
  if (fparams->sortby != NULL) {
    sprintf(buffer, "%ssortby=%s", is_first_param ? "?" : "&", fparams->sortby);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  append_paging_params(url, limit, offset, is_first_param);

  return url;
}

typedef struct {
  int channel_ids_count;
  const char **channel_ids;
  const char *game;
  const char *stream_type;
  const char *language;
} streams_params;

string_t *streams_url_builder(void *params, int limit, int offset) {
  streams_params *sparams = (streams_params *)params;
  bool is_first_param = true;
  char buffer[512];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/");

  // Append game.
  if (sparams->game != NULL) {
    sprintf(buffer, "?game=%s", sparams->game);
    string_append((void *)buffer, strlen(buffer), url);
    is_first_param = false;
  }

  // Append channel ids.
  if (sparams->channel_ids_count > 0) {
    string_t *channels = string_joined(sparams->channel_ids_count, sparams->channel_ids, ",");
    sprintf(buffer, "%schannel=%s", is_first_param ? "?" : "&", channels->ptr);
    string_append((void *)buffer, strlen(buffer), url);
    string_free(channels);
    is_first_param = false;
  }

  // Append stream type.
  if (sparams->stream_type != NULL) {
    sprintf(buffer, "%sstream_type=%s", is_first_param ? "?" : "&", sparams->stream_type);
    string_append((void *)buffer, strlen(buffer), url);
    is_first_param = false;
  }

  // Append language.
  if (sparams->language != NULL) {
    sprintf(buffer, "%slanguage=%s", is_first_param ? "?" : "&", sparams->language);
    string_append((void *)buffer, strlen(buffer), url);
    is_first_param = false;
  }

  append_paging_params(url, limit, offset, is_first_param);

  return url;
}

string_t *featured_streams_url_builder(void *params, int limit, int offset) {
  // Featured streams endpoint doesn't have any additional parameters.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/featured");
  append_paging_params(url, limit, offset, true);

  return url;
}

typedef struct {
  const char *query;
} channel_search_params;

string_t *channel_search_url_builder(void *params, int limit, int offset) {
  channel_search_params *csparams = (channel_search_params *)params;
  char buffer[256];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/search/channels");

  // Append query.
  const char *urlencoded_query = url_encode(csparams->query);
  string_append((void *)"?query=", strlen("?query="), url);
  string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

  append_paging_params(url, limit, offset, false);
  return url;
}

typedef struct {
  const char *query;
  stream_filter filter;
} stream_search_params;

string_t *stream_search_url_builder(void *params, int limit, int offset) {
  stream_search_params *ssparams = (stream_search_params *)params;
  char buffer[256];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/search/streams");

  // Append query.
  const char *urlencoded_query = url_encode(ssparams->query);
  string_append((void *)"?query=", strlen("?query="), url);
  string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

  // Append stream type filter.
  switch (ssparams->filter) {
    case none:
      break;
    case hls:
      string_append((void *)"&hls=true", strlen("&hls=true"), url);
      break;
    case rtmp:
      string_append((void *)"&hls=false", strlen("&hls=false"), url);
      break;
  }

  append_paging_params(url, limit, offset, false);
  return url;
}

typedef struct { 
  const char *query;
  bool live; 
} game_search_params;

string_t *game_search_url_builder(void *params, int limit, int offset) {
  game_search_params *gsparams = (game_search_params *)params;
  char buffer[256];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/search/games");

  // Append query.
  const char *urlencoded_query = url_encode(gsparams->query);
  string_append((void *)"?query=", strlen("?query="), url);
  string_append((void *)urlencoded_query, strlen(urlencoded_query), url);

  // Append stream type filter.
  if (gsparams->live) {
      string_append((void *)"&live=true", strlen("&live=true"), url);
  } else {
      string_append((void *)"&live=false", strlen("&live=false"), url);
  }

  return url;
}

typedef void *(*parser_func)(json_value *);

void **get_page(const char *client_id, page_url_builder builder, void *params, int limit, int offset, const char *values_key, parser_func parser, int *size, int *total) {
  string_t *url = builder(params, limit, offset);
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  void **elements = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, values_key) == 0) {
      json_value *elements_value = value->u.object.values[x].value;
      int elements_length = elements_value->u.array.length;
      if (elements_length == 0) {
        *size = 0;
        break;
      }

      elements = parse_json_array(elements_value, size, parser);
    } else if (strcmp(value->u.object.values[x].name, "_total") == 0) {
      *total = value->u.object.values[x].value->u.integer;
    }
  }

  json_value_free(value);
  return elements;
}

void **get_all_pages(const char *client_id, page_url_builder builder, void *params, const char *values_key, parser_func parser, bool ignore_totals, int *size) {
  const int PAGE_SIZE = 20;

  int count = 0;
  int total = 0;
  int offset = 0;
  void **elements = NULL;

  do {
    void **page = get_page(client_id, builder, params, PAGE_SIZE, offset, values_key, parser, &count, &total);

    // Don't do anything if there are 0 follows.
    if (count == 0) {
      if (offset + count < total) {
        total = (offset + count);
      }
      break;
    }

    // (Re)allocate memory to store next page.
    if (offset == 0) {
      elements = malloc(sizeof(void *) * count);
    } else {
      elements = realloc(elements, sizeof(void *) * (offset + count));
    }

    if (elements == NULL) {
      fprintf(stderr, "Failed to allocate memory for next page.");
      exit(EXIT_FAILURE);
    }

    // Copy page's content to the overall storage.
    memcpy(&elements[offset], page, sizeof(void *) * count);

    // Offset to the next page.
    offset += count;

    // Free current page data.
    free(page);
  } while ((ignore_totals && (count == PAGE_SIZE)) || (offset < total));

  // Return the whole list.
  *size = ignore_totals ? offset : total;
  return elements;
}

/** API **/

twitch_user *twitch_v5_get_user(const char *client_id, const char *id) {
  twitch_user *user = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)id, strlen(id), url);

  // Get JSON.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  if (value->type != json_object) {
    fprintf(stderr, "Wrong JSON type of returned value.");
    exit(EXIT_FAILURE);
  }

  user = parse_user(value);
  json_value_free(value);

  return user;
}

twitch_user *twitch_v5_get_user_by_username(const char *client_id, const char *username) {
  // Output value.
  twitch_user *user = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users?login=");
  string_append((void *)username, strlen(username), url);

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  int length = value->u.object.length;

  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      json_value *user_object = users_value->u.array.values[0];
      user = parse_user(user_object);
    }
  }

  json_value_free(value);
  return user;
}

twitch_user **twitch_v5_get_users(const char *client_id, int usernames_count, const char **usernames, int *total) {
  char buffer[2048];

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users");

  // Append usernames.
  if (usernames_count > 0) {
    string_t *usernames_string = string_joined(usernames_count, usernames, ",");
    sprintf(buffer, "?login=%s", usernames_string->ptr);
    string_append((void *)buffer, strlen(buffer), url);
    string_free(usernames_string);
  }

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  // Extract the relevant fields.
  twitch_user **users = NULL;

  int length = value->u.object.length;
  for (int x = 0; x < length; x++) {
    if (strcmp(value->u.object.values[x].name, "users") == 0) {
      json_value *users_value = value->u.object.values[x].value;
      int users_length = users_value->u.array.length;
      if (users_length == 0) {
        break;
      }

      users = (twitch_user **)parse_json_array(users_value, total, &parse_user);
    }
  }

  json_value_free(value);
  return users;
}

twitch_follow *twitch_v5_check_user_follow(const char *client_id, const char *user_id, const char *channel_id) {
  // Output value.
  twitch_follow *follow = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/users/");
  string_append((void *)user_id, strlen(user_id), url);
  string_append((void *)"/follows/channels", strlen("/follows/channels"), url);
  string_append((void *)channel_id, strlen(channel_id), url);

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  if (value == NULL) {
    return NULL;
  }

  follow = parse_follow(value);
  json_value_free(value);

  return follow;
}

twitch_follow **twitch_v5_get_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int limit, int offset, int *size, int *total) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_page(client_id, &user_follows_url_builder, (void *)&params, limit, offset, "follows", &parse_follow, size, total);
  return follows;
}

twitch_follow** twitch_v5_get_all_user_follows(const char *client_id, const char *user_id, const char *direction, const char *sortby, int *size) {
  follows_params params = {
    .user_id = user_id,
    .direction = direction,
    .sortby = sortby
  };

  twitch_follow **follows = (twitch_follow **)get_all_pages(client_id, &user_follows_url_builder, (void *)&params, "follows", &parse_follow, false, size);
  return follows;
}

twitch_stream **twitch_v5_get_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int limit, int offset, int *size, int *total) {
  streams_params params = {
    .channel_ids_count = channel_ids_count,
    .channel_ids = channel_ids,
    .game = game,
    .stream_type = stream_type,
    .language = language
  };

  twitch_stream **streams = (twitch_stream **)get_page(client_id, &streams_url_builder, (void *)&params, limit, offset, "streams", &parse_stream, size, total);
  return streams;
}

twitch_stream** twitch_v5_get_all_streams(const char *client_id, int channel_ids_count, const char **channel_ids, const char *game, const char *stream_type, const char* language, int *size) {
  streams_params params = {
    .channel_ids_count = channel_ids_count,
    .channel_ids = channel_ids,
    .game = game,
    .stream_type = stream_type,
    .language = language
  };

  twitch_stream **streams = (twitch_stream **)get_all_pages(client_id, &streams_url_builder, (void *)&params, "streams", &parse_stream, false, size);
  return streams;
}

twitch_stream *twitch_v5_get_stream_by_user(const char *client_id, const char *channel_id, const char *stream_type) {
  twitch_stream *stream = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/");

  // Append Channel ID params.
  string_append((void *)channel_id, strlen(channel_id), url);

  // Append type.
  if (stream_type != NULL) {
    char buffer[24];
    sprintf(buffer, "?stream_type=%s", stream_type);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);
  string_free(url);

  for (int prop_ind = 0; prop_ind < value->u.object.length; prop_ind++) {
    if (strcmp(value->u.object.values[prop_ind].name, "stream") == 0) {
      json_value *stream_value = value->u.object.values[prop_ind].value;
      if (stream_value->type == json_null) {
        break;
      }

      stream = (twitch_stream *)parse_stream(stream_value);
    }
  }

  json_value_free(value);
  return stream;
}

twitch_summary *twitch_v5_get_summary(const char *client_id, const char *game) {
  twitch_summary *summary = NULL;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/streams/summary");

  // Append game name.
  if (game != NULL) {
    char buffer[256];
    const char *urlencoded_game = url_encode(game);
    sprintf(buffer, "?game=%s", urlencoded_game);
    string_append((void *)buffer, strlen(buffer), url);
  }

  // Get the output.
  json_value *value = twitch_v5_get_json(client_id, url->ptr);

  summary = (twitch_summary *)parse_summary(value);

  json_value_free(value);
  return summary;
}

twitch_featured_stream **twitch_v5_get_featured_streams(const char *client_id, int limit, int offset, int *size) {
  int total = 0;
  twitch_featured_stream **streams = (twitch_featured_stream **)get_page(client_id, &featured_streams_url_builder, NULL, limit, offset, "featured", &parse_featured_stream, size, &total);
  return streams;
}

twitch_featured_stream **twitch_v5_get_all_featured_streams(const char *client_id, int *size) {
  twitch_featured_stream **streams = (twitch_featured_stream **)get_all_pages(client_id, &featured_streams_url_builder, NULL, "featured", &parse_featured_stream, true, size);
  return streams;
}

twitch_channel **twitch_v5_search_channels(const char *client_id, const char *query, int limit, int offset, int *size, int *total) {
  channel_search_params params = {
    .query = query
  };

  twitch_channel **channels = (twitch_channel **)get_page(client_id, &channel_search_url_builder, &params, limit, offset, "channels", &parse_channel, size, total);
  return channels;
}

twitch_channel **twitch_v5_search_all_channels(const char *client_id, const char *query, int *size) {
  channel_search_params params = {
    .query = query
  };

  twitch_channel **channels = (twitch_channel **)get_all_pages(client_id, &channel_search_url_builder, &params, "channels", &parse_channel, false, size);
  return channels;
}
 
twitch_stream **twitch_v5_search_streams(const char *client_id, const char *query, stream_filter filter, int limit, int offset, int *size, int *total) {
  stream_search_params params = {
    .query = query,
    .filter = filter
  };

  twitch_stream **streams = (twitch_stream **)get_page(client_id, &stream_search_url_builder, &params, limit, offset, "streams", &parse_stream, size, total);
  return streams;
}

twitch_stream **twitch_v5_search_all_streams(const char *client_id, const char *query, stream_filter filter, int *size) {
  stream_search_params params = {
    .query = query,
    .filter = filter
  };

  twitch_stream **streams = (twitch_stream **)get_all_pages(client_id, &stream_search_url_builder, &params, "streams", &parse_stream, false, size);
  return streams;
}

twitch_game **twitch_v5_search_games(const char *client_id, const char *query, bool live, int *size) {
  game_search_params params = {
    .query = query,
    .live = live
  };

  int total = 0;
  twitch_game **games = (twitch_game **)get_page(client_id, &game_search_url_builder, &params, 0, 0, "games", &parse_game, size, &total);
  return games;
}

