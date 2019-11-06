#include <curl/curl.h>
#include <stdio.h>

#include "twitch.h"
#include "string_utils.h"
#include "array_utils.h"

/** Main **/

void *channel_id_from_follow(void *src) {
  twitch_follow *follow = (twitch_follow *)src;
  char buffer[64];

  sprintf(buffer, "%lld", follow->channel->id);
  return immutable_string_copy(buffer);
}

void test_search_channels(const char *client_id, const char *query, int page_size) {
  printf("*** Searching for channels with query '%s' ***\n", query);

  int size = 0, total = 0;

  twitch_channel **channels = twitch_v5_search_channels(client_id, query, page_size, 0, &size, &total);
  if (channels != NULL && size > 0) {
    printf("Downloaded: %d, Total: %d\n", size, total);
    for (int index = 0; index < size; index++) {
      printf("ID: %lld,\n  Name: %s,\n  Status: %s\n", channels[index]->id, channels[index]->name, channels[index]->status);
    }
  }

  twitch_channel_list_free(size, channels);
}

void test_search_all_channels(const char *client_id, const char *query) {
  printf("*** Searching for all channels with query '%s' ***\n", query);

  int size = 0;

  twitch_channel **channels = twitch_v5_search_all_channels(client_id, query, &size);
  if (channels != NULL && size > 0) {
    printf("Downloaded: %d\n", size);
    for (int index = 0; index < size; index++) {
      printf("ID: %lld,\n  Name: %s,\n  Status: %s\n", channels[index]->id, channels[index]->name, channels[index]->status);
    }
  }

  twitch_channel_list_free(size, channels);
}
twitch_user *test_get_user(const char *client_id, const char *username) {
  printf("*** Getting user data for '%s' ***\n", username);

  twitch_user *user = twitch_v5_get_user_by_username(client_id, username);
  printf("Username: %s, ID: %s, Display Name: %s, Created At: %s, Updated At: %s, Type: %s\n", user->name, user->id, user->display_name, user->created_at, user->updated_at, user->type);

  return user;
}

void test_is_following(const char *client_id, const char *user_id, const char *channel_id) {
  printf("*** Checking if user '%s' follows channel with ID '%s' ***\n", user_id, channel_id);

  twitch_follow *specific_follow = twitch_v5_check_user_follow(client_id, user_id, channel_id);
  if (specific_follow != NULL) {
    printf("  Yes\n");
    twitch_follow_free(specific_follow);
  } else {
    printf("  No\n");
  }
}

void test_get_users(const char *client_id, int count, const char *usernames[]) {
  printf("*** Getting users ***\n");

  int usercount = 0;
  twitch_user **users = twitch_v5_get_users(client_id, count, (const char **)usernames, &usercount);

  if (users != NULL && usercount > 0) {
    for (int index = 0; index < usercount; index++) {
      twitch_user *usr = users[index];
      printf("Username: %s\n  ID: %s\n  Display Name: %s\n  Created At: %s\n  Updated At: %s\n  Type: %s\n", usr->name, usr->id, usr->display_name, usr->created_at, usr->updated_at, usr->type);
    }
    twitch_users_list_free(users, usercount);
  }
}

void test_get_follows(const char *client_id, const char *user_id) {
  int total = 0;
  twitch_follow **follows = twitch_v5_get_all_user_follows(client_id, user_id, NULL, NULL, &total);

  for (int idx = 0; idx < total; idx++) {
    twitch_follow *follow = follows[idx];
    printf("ID: %lld\n  Name: %s\n  Status: %s\n", follow->channel->id, follow->channel->name, follow->channel->status);
  }

  twitch_follows_list_free(follows, total);
}

int main(int argc, char **argv) {
  const int PAGE_SIZE = 20;
  const char *CLIENT_ID = "454q3qk5jh0rzgps78fnxrwc5u1i8t";

  // Global cURL init.
  curl_global_init(CURL_GLOBAL_ALL);

  // Get user by name.
  twitch_user *user = test_get_user(CLIENT_ID, "cog1to");

  // Check following.
  test_is_following(CLIENT_ID, user->id, "129454141");

  // Get multiple users.
  const char *usernames[] = {
    "cog1to",
    "dyingcamel",
    "scumcreep",
    "560-56056089"
  };
  test_get_users(CLIENT_ID, 4, usernames);

  // Get all follows.
  int total = 0;
  twitch_follow **follows = twitch_v5_get_all_user_follows(CLIENT_ID, user->id, NULL, NULL, &total);

  for (int idx = 0; idx < total; idx++) {
    twitch_follow *follow = follows[idx];
    printf("ID: %lld\n  Name: %s\n  Status: %s\n", follow->channel->id, follow->channel->name, follow->channel->status);
  }

  // Test getting streams.
  printf("*** Getting streams ***\n");

  // Grab all channel IDs.
  char **channel_ids = (char **)pointer_array_map((void **)follows, total, &channel_id_from_follow);
  int streams_count = 0;
  twitch_stream **streams = twitch_v5_get_all_streams(CLIENT_ID, total, (const char **)channel_ids, NULL, NULL, NULL, &streams_count);

  for (int idx = 0; idx < streams_count; idx++) {
    twitch_stream *stream = streams[idx];
    printf("ID: %lld\n  Channel ID: %lld\n  Name: %s\n  Game: %s\n", stream->id, stream->channel->id, stream->channel->name, stream->game);
  }

  twitch_follows_list_free(follows, total);
  pointer_array_free(total, (void **)channel_ids, (void(*)(void*))&free);

  // Test getting stream by channel ID.
  if (streams_count > 0) {
    printf("*** Getting stream by channel ID ***\n");

    char channel_id[32];
    sprintf(channel_id, "%lld", streams[0]->channel->id);
    twitch_stream *stream_by_user = twitch_v5_get_stream_by_user(CLIENT_ID, channel_id, NULL); 

    if (stream_by_user != NULL) {
      printf("ID: %lld\n  Name: %s\n Game: %s\n  Status: %s\n", stream_by_user->id, stream_by_user->channel->name, stream_by_user->game, stream_by_user->channel->status);
      twitch_stream_free(stream_by_user);
    }
  }

  twitch_streams_list_free(streams, streams_count);

  // Test stream summary.
  printf("*** Getting summary for 'Overwatch' ***\n");

  twitch_summary *summary = twitch_v5_get_summary(CLIENT_ID, "Overwatch");
  printf("Viewers: %d, Channels: %d\n", summary->viewers, summary->channels);
  twitch_summary_free(summary);

  // Test featured streams.
  printf("*** Getting featured streams ***\n");
  
  int featured_count = 0;
  twitch_featured_stream **featured = twitch_v5_get_all_featured_streams(CLIENT_ID, &featured_count);
  if (featured != NULL && featured_count > 0) {
    for (int idx = 0; idx < featured_count; idx++) {
      printf("Channel Name: %s\n  Title: %s\n", featured[idx]->stream->channel->name, featured[idx]->title);
    }
    twitch_featured_stream_list_free(featured_count, featured);
  }
  
  // Test channel search.
  test_search_channels(CLIENT_ID, "starcraft", 20);
  test_search_all_channels(CLIENT_ID, "capitalize"); 

  return 0;
}

