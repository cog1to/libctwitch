/**
 * Simple Twitch API client to show off the library's abilities.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#include <curl/curl.h>
#include <stdio.h>
#include <stdbool.h>
#include "v5.h"
#include "ctwitch.h"

/** A little bit of a hack - We reference library's internal methods here just to save some space. **/

extern char *immutable_string_copy(const char *src);
extern void **pointer_array_map(void **src, size_t src_count, void *(*getter)(void *));
extern void pointer_array_free(int count, void **src, void(*deinit)(void *));

/** Helpers **/

void *channel_id_from_follow(void *src) {
  twitch_follow *follow = (twitch_follow *)src;
  char buffer[64];

  sprintf(buffer, "%lld", follow->channel->id);
  return immutable_string_copy(buffer);
}

/** Command-line data **/

// List of supported commands.
typedef enum {
  featured,
  search_games,
  search_streams,
  user,
  live_follows
} command_type;

// Command handler function.
typedef void(*command_handler)(const char *, int, const char **);

// Command specification.
typedef struct {
  command_type command;
  char name[32];
  char description[128];
  bool has_parameter;
  command_handler handler;
} command_spec;

/** Commands **/

/**
 * Prints usage.
 *
 * @param count Number of supported commands.
 * @param commands List of commands.
 */
void print_usage(int count, command_spec *commands) {
  printf("\nUsage:\n  twitch-remote <command> <argument> [options]\n\n");

  //Commands
  printf("Commands:\n");

  for (int index = 0; index < count; index++) {
    command_spec command = commands[index];

    // Format command usage string.
    char command_usage[32];
    sprintf(command_usage, "%s%s", command.name, command.has_parameter ? " <query>" : "");

    printf("  %-25s\t%s\n", command_usage, command.description);
  }

  // Options
  printf("\nOptions:\n");
  printf("  %-25s\t(REQUIRED) Registered Twitch API Client ID string.\n", "--client-id=CLIENT_ID\t");
}

/**
 * Gets parameter value from provided options array. Options are expected to be in a format
 *   --[param-name]=[param-value].
 *
 * @param param_name Name of the parameter to get value for.
 * @param options_count Total number of option strings.
 * @param options List of option strings.
 *
 * @return String value of given parameter, or NULL, if matching parameter is not found.
 *   Caller should free it manually afterwards.
 */
char *get_param(const char *param_name, int options_count, const char **options) {
  char sparam_name[16];
  char sparam_value[32];

  for (int idx = 0; idx < options_count; idx++) {
    int parsed = sscanf(options[idx], "--%[^=]=%s", sparam_name, sparam_value);
    if (parsed > 0 && strcmp(param_name, sparam_name) == 0) {
      char *output = malloc(strlen(sparam_value));
      strcpy(output, sparam_value);
      return output;
    }
  }

  return NULL;
}

/**
 * Gets and prints the featured streams.
 *
 * @param param Parameter string (optional)
 * @param options_count Number of provided options.
 * @param options List of option strings.
 */
void get_featured(const char *param, int options_count, const char **options) {
  char *CLIENT_ID = get_param("client-id", options_count, options);
  if (CLIENT_ID == NULL) {
    fprintf(stderr, "Error: client ID should be provided with '--client-id=' option.\n");
    exit(EXIT_FAILURE);
  }

  int featured_count = 0;
  twitch_featured_stream **featured = twitch_v5_get_all_featured_streams(CLIENT_ID, &featured_count);
  if (featured != NULL && featured_count > 0) {
    for (int idx = 0; idx < featured_count; idx++) {
      twitch_featured_stream *stream = featured[idx];
      printf(
        "Channel Name: %s\n  Title: %s\n  Game: %s\n  Viewers: %d\n  URL: %s\n  Text: %s\n",
        stream->stream->channel->name,
        stream->title,
        stream->stream->game,
        stream->stream->viewers,
        stream->stream->channel->url,
        stream->text
      );
    }
    twitch_featured_stream_list_free(featured_count, featured);
  }

  free(CLIENT_ID);
}

/**
 * Gets and prints username data.
 *
 * @param username Username to get user info for.
 * @param options_count Number of CLI arguments.
 * @param options List of arguments.
 */
void get_user(const char *username, int options_count, const char **options) {
  char *CLIENT_ID = get_param("client-id", options_count, options);
  if (CLIENT_ID == NULL) {
    fprintf(stderr, "Error: client ID should be provided with '--client-id=' option.\n");
    exit(EXIT_FAILURE);
  }

  twitch_user *user = twitch_v5_get_user_by_username(CLIENT_ID, username);
  if (user != NULL) {
    printf(
      "Username: %s\n  ID: %s\n  Display Name: %s\n  Created At: %s\n  Updated At: %s\n  Type: %s\n",
      user->name,
      user->id,
      user->display_name,
      user->created_at,
      user->updated_at,
      user->type
    );
    twitch_user_free(user);
  } else {
    fprintf(stderr, "Error: user with login '%s' not found\n", username);
  }

  free(CLIENT_ID);
}

/**
 * Searches for games matching given query.
 *
 * @param name Game's name qeury.
 * @param options_count Number of command line arguments.
 * @param options List of command line arguments.
 */
void get_games(const char *name, int options_count, const char **options) {
  char *CLIENT_ID = get_param("client-id", options_count, options);
  if (CLIENT_ID == NULL) {
    fprintf(stderr, "Error: client ID should be provided with '--client-id=' option.\n");
    exit(EXIT_FAILURE);
  }

  int size = 0;
  twitch_game **games = twitch_v5_search_games(CLIENT_ID, name, false, &size);
  if (games != NULL && size > 0) {
    for (int index = 0; index < size; index++) {
      twitch_game *game = games[index];
      printf(
        "ID: %lld\n  Name: %s\n  Popularity: %d\n  Giantbomb ID: %lld\n",
        game->id,
        game->name,
        game->popularity,
        game->giantbomb_id);
    }
    twitch_game_list_free(size, games);
  }

  free(CLIENT_ID);
}

/**
 * Searches for live streams matching given query.
 *
 * @param query Query string to match.
 * @param options_count Number of command line arguments.
 * @param options List of command line arguments.
 */
void get_streams(const char *query, int options_count, const char **options) {
  char *CLIENT_ID = get_param("client-id", options_count, options);
  if (CLIENT_ID == NULL) {
    fprintf(stderr, "Error: client ID should be provided with '--client-id=' option.\n");
    exit(EXIT_FAILURE);
  }

  int size = 0;
  twitch_stream **streams = twitch_v5_search_all_streams(CLIENT_ID, query, none, &size);
  if (streams != NULL && size > 0) {
    printf("Streams found: %d\n", size);
    for (int index = 0; index < size; index++) {
      twitch_stream *stream = streams[index];
      printf(
        "ID: %lld\n  Game: %s\n  Channel: %s\n  Status: %s\n  URL: %s\n",
        stream->id,
        stream->game,
        stream->channel->name,
        stream->channel->status,
        stream->channel->url
      );
    }
    twitch_stream_list_free(size, streams);
  }

  free(CLIENT_ID);
}

/**
 * Searches for live streams from given user's follows list.
 *
 * @param name Name of the user for which to get follows.
 * @param options_count Number of command line arguments.
 * @param options List of command line arguments.
 */
void get_live_follows(const char *username, int options_count, const char **options) {
  char *CLIENT_ID = get_param("client-id", options_count, options);
  if (CLIENT_ID == NULL) {
    fprintf(stderr, "Error: client ID should be provided with '--client-id=' option.\n");
    exit(EXIT_FAILURE);
  }

  // Find user by login name to get their user ID.
  twitch_user *user = twitch_v5_get_user_by_username(CLIENT_ID, username);
  if (user == NULL) {
    fprintf(stderr, "Error: user with login '%s' not found.\n", username);
    return;
  }

  // Get all user's follows.
  int follows_total = 0;
  twitch_follow **follows = twitch_v5_get_all_user_follows(CLIENT_ID, user->id, NULL, NULL, &follows_total);
  if (follows == NULL) {
    twitch_user_free(user);
    fprintf(stderr, "Error: failed to get user's follows.\n");
    return;
  }

  // Get all streams from user's follows.
  char **channel_ids = (char **)pointer_array_map((void **)follows, follows_total, &channel_id_from_follow);
  int streams_count = 0;
  twitch_stream **streams = twitch_v5_get_all_streams(
    CLIENT_ID,
    follows_total,
    (const char **)channel_ids,
    NULL,
    NULL,
    NULL,
    &streams_count
  );

  // Print stream data.
  if (streams_count > 0 && streams != NULL) {
    for (int idx = 0; idx < streams_count; idx++) {
      twitch_stream *stream = streams[idx];
      printf(
        "ID: %lld\n  Game: %s\n  Channel: %s\n  Status: %s\n  URL: %s\n",
        stream->id,
        stream->game,
        stream->channel->name,
        stream->channel->status,
        stream->channel->url
      );
    }
    twitch_stream_list_free(streams_count, streams);
  }

  // Cleanup.
  twitch_user_free(user);
  twitch_follows_list_free(follows, follows_total);
  pointer_array_free(follows_total, (void **)channel_ids, (void(*)(void*))&free);
  free(CLIENT_ID);
}

/** Main **/

int main(int argc, char **argv) {
  command_spec commands[] = {
    {
      .command = featured,
      .name = "featured",
      .description = "Prints out featured streams.",
      .has_parameter = false,
      .handler = &get_featured
    },
    {
      .command = search_games,
      .name = "search-games",
      .description = "Searches for games matching given string.",
      .has_parameter = true,
      .handler = &get_games
    },
    {
      .command = search_streams,
      .name = "search-streams",
      .description = "Searches for live streams matching given query string.",
      .has_parameter = true,
      .handler = &get_streams
    },
    {
      .command = user,
      .name = "user",
      .description = "Prints info about user with given username.",
      .has_parameter = true,
      .handler = &get_user
    },
    {
      .command = live_follows,
      .name = "live-follows",
      .description = "Prints out live streams that given user is following.",
      .has_parameter = true,
      .handler = &get_live_follows
    }
  };

  int commands_count = sizeof(commands)/sizeof(command_spec);

  // No command: just print usage and exit.
  if (argc < 2) {
    print_usage(commands_count, commands);
  }

  // If there's a command, try to match it with one of the known ones, and execute it's handler.
  for (int cmd_idx = 0; cmd_idx < commands_count; cmd_idx++) {
    command_spec command = commands[cmd_idx];
    if (strcmp(argv[1], command.name) == 0) {
      if (command.handler != NULL) {
        // Don't forget to initialize the library!
        twitch_init();

        // Handle the command.
        if (command.has_parameter) {
          command.handler(argv[2], argc - 3, (const char **)(&argv[3]));
        } else {
          command.handler(NULL, argc - 2, (const char **)(&argv[2]));
        }
        return 0;
      }
    }
  }

  // If command wasn't found, just print usage.
  print_usage(commands_count, commands);
}

