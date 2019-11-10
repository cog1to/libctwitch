#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "data.h"

/** URL builders  **/

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

/** API **/

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

