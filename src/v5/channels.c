#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

#include "utils/string_utils.h"
#include "utils/network_utils.h"
#include "utils/parser.h"
#include "json/json.h"

#include <ctwitch/v5/data.h>

/** URL builders  **/

typedef struct {
  const char *channel_id;
  const char *direction;
} channel_followers_params;

string_t *channel_followers_url_builder(
	void *params,
	int limit,
	const char *cursor
) {
  char buffer[128];
  bool is_first_param = true;

  channel_followers_params *cfparams = (channel_followers_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/channels/");
  string_append((void *)cfparams->channel_id, strlen(cfparams->channel_id), url);
  string_append((void *)"/follows", strlen("/follows"), url);

  // Append direction.
  if (cfparams->direction != NULL) {
    sprintf(buffer, "?direction=%s", cfparams->direction);
    string_append(buffer, strlen(buffer), url);
    is_first_param = false;
  }

  append_cursor(url, cursor, is_first_param);

  return url;
}

typedef struct {
  const char *channel_id;
} channel_teams_params;

string_t *channel_teams_url_builder(void *params, int limit, int offset) {
  channel_teams_params *ctparams = (channel_teams_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/channels/");
  string_append((void *)ctparams->channel_id, strlen(ctparams->channel_id), url);
  string_append((void *)"/teams", strlen("/teams"), url);

  return url;
}

typedef struct {
  const char *channel_id;
  const char *broadcast_type;
  const char *language;
  const char *sort;
} channel_videos_params;

string_t *channel_videos_url_builder(void *params, int limit, int offset) {
  char buffer[128];
  channel_videos_params *cvparams = (channel_videos_params *)params;

  // Construct the link.
  string_t *url = string_init_with_value("https://api.twitch.tv/kraken/channels/");
  string_append((void *)cvparams->channel_id, strlen(cvparams->channel_id), url);
  string_append((void *)"/videos", strlen("/videos"), url);

  append_paging_params(url, limit, offset, true);

  // Append broadcast type filter.
  if (cvparams->broadcast_type != NULL) {
    sprintf(buffer, "&broadcast_type=%s", cvparams->broadcast_type);
    string_append(buffer, strlen(buffer), url);
  }

  // Append language filter.
  if (cvparams->language != NULL) {
    sprintf(buffer, "&language=%s", cvparams->language);
    string_append(buffer, strlen(buffer), url);
  }

  // Append sorting.
  if (cvparams->sort != NULL) {
    sprintf(buffer, "&sort=%s", cvparams->sort);
    string_append(buffer, strlen(buffer), url);
  }

  return url;
}

/** API **/

twitch_v5_follower_list *twitch_v5_get_channel_followers(const char *client_id, const char *channel_id, int limit, const char *cursor, const char *direction, int *total, char **next_cursor) {
  channel_followers_params params = {
    .channel_id = channel_id,
    .direction = direction,
  };

  twitch_v5_follower_list *followers = twitch_v5_follower_list_alloc();
  followers->items = (twitch_v5_follower **)get_cursored_page(client_id, &channel_followers_url_builder, (void *)&params, limit, cursor, "follows", &parse_follower, &followers->count, next_cursor);
  return followers;
}

twitch_v5_follower_list *twitch_v5_get_all_channel_followers(const char *client_id, const char *channel_id, const char *direction) {
  channel_followers_params params = {
    .channel_id = channel_id,
    .direction = direction,
  };

  twitch_v5_follower_list *followers = twitch_v5_follower_list_alloc();
  followers->items = (twitch_v5_follower **)get_all_cursored_pages(client_id, &channel_followers_url_builder, (void *)&params, "follows", &parse_follower, &followers->count);
  return followers;
}

twitch_v5_team_list *twitch_v5_get_channel_teams(const char *client_id, const char *channel_id) {
  channel_teams_params params = {
    .channel_id = channel_id,
  };

  twitch_v5_team_list *teams = twitch_v5_team_list_alloc();
  teams->items = (twitch_v5_team **)get_page(client_id, &channel_teams_url_builder, (void *)&params, 0, 0, "teams", &parse_team, &teams->count, NULL);
  return teams;
}

twitch_v5_community_list *twitch_v5_get_channel_communities(const char *client_id, const char *channel_id) {
  channel_teams_params params = {
    .channel_id = channel_id,
  };

  twitch_v5_community_list *communities = twitch_v5_community_list_alloc();
  communities->items = (twitch_v5_community **)get_page(client_id, &channel_teams_url_builder, (void *)&params, 0, 0, "communities", &parse_community, &communities->count, NULL);
  return communities;
}

twitch_v5_video_list *twitch_v5_get_channel_videos(const char *client_id, const char *channel_id, int limit, int offset, const char *broadcast_type, const char *language, char *sort, int* total) {
  channel_videos_params params = {
    .channel_id = channel_id,
    .broadcast_type = broadcast_type,
    .language = language,
    .sort = sort,
  };

  twitch_v5_video_list *videos = twitch_v5_video_list_alloc();
  videos->items = (twitch_v5_video **)get_page(client_id, &channel_videos_url_builder, (void *)&params, limit, offset, "videos", &parse_video, &videos->count, total);
  return videos;
}

twitch_v5_video_list *twitch_v5_get_all_channel_videos(const char *client_id, const char *channel_id, const char *broadcast_type, const char *language, char *sort) {
  channel_videos_params params = {
    .channel_id = channel_id,
    .broadcast_type = broadcast_type,
    .language = language,
    .sort = sort,
  };

  twitch_v5_video_list *videos = twitch_v5_video_list_alloc();
  videos->items = (twitch_v5_video **)get_all_pages(client_id, &channel_videos_url_builder, (void *)&params, "videos", &parse_video, false, &videos->count);
  return videos;
}

