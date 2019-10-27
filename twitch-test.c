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

int main(int argc, char **argv) {
  const int PAGE_SIZE = 20;
  const char *CLIENT_ID = "454q3qk5jh0rzgps78fnxrwc5u1i8t";

  // Global cURL init.
  curl_global_init(CURL_GLOBAL_ALL);

  twitch_user *user = twitch_v5_get_user("cog1to", CLIENT_ID);
  printf("Username: %s, ID: %s\n", user->name, user->id);

  printf("Getting follows\n");

  int total = 0;
  twitch_follow **follows = twitch_v5_get_all_user_follows(CLIENT_ID, user->id, NULL, NULL, &total);

  printf("Printing follows\n");

  for (int idx = 0; idx < total; idx++) {
    twitch_follow *follow = follows[idx];
    printf("ID: %lld, Name: %s, Status: %s\n", follow->channel->id, follow->channel->name, follow->channel->status);
  }

  printf("Getting streams.\n");

  // Grab all channel IDs.
  char **channel_ids = (char **)pointer_array_map((void **)follows, total, &channel_id_from_follow);

  int streams_count = 0;
  twitch_stream **streams = twitch_v5_get_all_streams(CLIENT_ID, total, (const char **)channel_ids, NULL, NULL, NULL, &streams_count);
  //twitch_stream **streams = twitch_v5_get_all_streams(CLIENT_ID, 0, NULL, "Overwatch", NULL, NULL, &streams_count);

  for (int idx = 0; idx < streams_count; idx++) {
    twitch_stream *stream = streams[idx];
    printf("ID: %lld, Name: %s, Game: %s\n", stream->id, stream->channel->name, stream->game);
  }

  twitch_follows_list_free(follows, total);
  twitch_streams_list_free(streams, streams_count);
  pointer_array_free(total, (void **)channel_ids, (void(*)(void*))&free);
  twitch_user_free(user);

  return 0;
}

