#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"
#include "data.h"

/** URL builders  **/

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

/** API **/

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
