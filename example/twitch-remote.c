/**
 * Simple Twitch API client to show off the library's abilities.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#include <curl/curl.h>
#include <stdio.h>
#include <stdbool.h>

#include <ctwitch/auth.h>
#include <ctwitch/v5.h>
#include <ctwitch/helix.h>
#include <ctwitch/ctwitch.h>

/**
 * A little bit of a hack - We reference library's internal methods here just
 * to save some space.
 **/

extern char *immutable_string_copy(const char *src);
extern void **pointer_array_map(
	void **src,
	size_t src_count,
	void *(*getter)(void *)
);
extern void pointer_array_free(int count, void **src, void(*deinit)(void *));

/** Helpers **/

void *channel_id_from_follow(void *src) {
	twitch_v5_follow *follow = (twitch_v5_follow *)src;
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
	live_follows,
	top_games,
	channel_follows,
	channel_teams,
	channel_communities,
	channel_videos,
	team,
	token,
	helix_user,
	helix_follows,
	helix_live_follows,
	helix_channel_follows,
	helix_live_channel_follows
} command_type;

// Command handler function  type.
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
	printf("\nUsage:\n	twitch-remote <command> <argument> [options]\n\n");

	//Commands
	printf("Commands:\n");

	for (int index = 0; index < count; index++) {
		command_spec command = commands[index];

		// Format command usage string.
		char command_usage[32];
		sprintf(
			command_usage,
			"%s%s",
			command.name,
			command.has_parameter ? " <query>" : ""
		);

		printf("	%-25s\t%s\n", command_usage, command.description);
	}

	// Options
	printf("\nOptions:\n");
	printf("	%-25s\t(REQUIRED) Registered Twitch API Client ID string.\n",
		"--client-id=CLIENT_ID\t"
	);
}

/**
 * Gets parameter value from provided options array. Options are expected to be
 * in a format --[param-name]=[param-value].
 *
 * @param param_name Name of the parameter to get value for.
 * @param options_count Total number of option strings.
 * @param options List of option strings.
 *
 * @return String value of given parameter, or NULL, if matching parameter is
 * not found. Caller should free it manually afterwards.
 */
char *get_param(
	const char *param_name,
	int options_count,
	const char **options
) {
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

/** Gets client_id parameter from provided options array.
 *
 * @param options_count Number of options in the options array.
 * @param options Options array.
 *
 * @return Client ID string or NULL if it's not provided.
 */
char *get_client_id(int options_count, const char **options) {
	char *client_id = get_param("client-id", options_count, options);
	if (client_id == NULL) {
		fprintf(
			stderr,
			"Error: client ID should be provided with '--client-id=' option.\n"
		);
		exit(EXIT_FAILURE);
	}

	return client_id;
}

/** Gets client_id parameter from provided options array.
 *
 * @param options_count Number of options in the options array.
 * @param options Options array.
 *
 * @return Client ID string or NULL if it's not provided.
 */
char *get_client_secret(int options_count, const char **options) {
	char *client_secret = get_param("client-secret", options_count, options);
	if (client_secret == NULL) {
		fprintf(
			stderr,
			"Error: Client secret should be provided with '--client-secret=' option.\n"
		);
		exit(EXIT_FAILURE);
	}

	return client_secret;
}

/** Gets user_token parameter from provided options array.
 *
 * @param options_count Number of options in the options array.
 * @param options Options array.
 * @param fail Exit the program with failure if it's not present.
 *
 * @return User access token string or NULL if it's not provided.
 */
char *get_user_token(int options_count, const char **options, int fail) {
	char *user_token = get_param("user-token", options_count, options);
	if (user_token == NULL && fail) {
		fprintf(
			stderr,
			"Error: User token should be provided with '--user_token=' option.\n"
		);
		exit(EXIT_FAILURE);
	}

	return user_token;
}

/**
 * Gets bearer token from arguments provided. If `--user-token` arg is present,
 * returns that. If `--client-secret` arg is present, tries to obtain app
 * access token using client credentials flow with given client id and client
 * secret.
 *
 * @param options_count Number of options in the options array.
 * @param options Options array.
 *
 * @return A valid bearer token, either app access or user access token.
 */
char *get_bearer_token(int options_count, const char **options) {
	char *BEARER = get_user_token(options_count, options, 0);
	twitch_app_access_token *token = NULL;

	if (BEARER == NULL) {
		char *CLIENT_ID = get_client_id(options_count, options);
		char *CLIENT_SECRET = get_client_secret(options_count, options);

		token = twitch_get_app_access_token(
			CLIENT_ID,
			CLIENT_SECRET
		);

		BEARER = token->token;
		free(CLIENT_SECRET);
		free(CLIENT_ID);
	}

	if (BEARER == NULL) {
		fprintf(
			stderr,
			"Error: client ID or user token should be provided with.\n"
		);
		exit(EXIT_FAILURE);
	}

	return BEARER;
}

/**
 * Searches for channel with given name and returns it's stringified ID.
 *
 * @param client_id Twitch API client ID.
 * @param query Search string.
 *
 * @return String containing channel's ID value, or null if it's not found.
 */
char *find_channel_id(char *client_id, const char *query) {
	int channels_total = 0;
	twitch_v5_channel *target = NULL;
	twitch_v5_channel_list *channels = twitch_v5_search_channels(
		client_id,
		query,
		20,
		0,
		&channels_total
	);

	if (channels->count > 0) {
		for (int index = 0; index < channels->count; index++) {
			twitch_v5_channel *channel = channels->items[index];
			if (strcmp(channel->name, query) == 0) {
				target = channel;
				break;
			}
		}
	}

	if (target == NULL) {
		twitch_v5_channel_list_free(channels);
		return NULL;
	}

	// Convert channel ID.
	char* channel_id = malloc(64 * sizeof(char));
	sprintf(channel_id, "%lld", target->id);

	twitch_v5_channel_list_free(channels);
	return channel_id;
}


/**
 * Gets and prints the featured streams.
 *
 * @param param Parameter string (optional)
 * @param options_count Number of provided options.
 * @param options List of option strings.
 */
void get_featured(const char *param, int options_count, const char **options) {
	char *CLIENT_ID = get_client_id(options_count, options);

	twitch_v5_featured_stream_list *featured = twitch_v5_get_all_featured_streams(
		CLIENT_ID
	);

	if (featured != NULL && featured->count > 0) {
		for (int idx = 0; idx < featured->count; idx++) {
			twitch_v5_featured_stream *stream = featured->items[idx];
			printf(
				"Channel Name: %s\n  Title: %s\n	Game: %s\n	Viewers: %d\n  URL: %s\n	Text: %s\n",
				stream->stream->channel->name,
				stream->title,
				stream->stream->game,
				stream->stream->viewers,
				stream->stream->channel->url,
				stream->text
			);
		}
		twitch_v5_featured_stream_list_free(featured);
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
	char *CLIENT_ID = get_client_id(options_count, options);
	twitch_v5_user *user = twitch_v5_get_user_by_username(CLIENT_ID, username);
	if (user != NULL) {
		printf(
			"Username: %s\n  ID: %lld\n  Display Name: %s\n  Created At: %s\n  Updated At: %s\n  Type: %s\n",
			user->name,
			user->id,
			user->display_name,
			user->created_at,
			user->updated_at,
			user->type
		);
		twitch_v5_user_free(user);
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
	char *CLIENT_ID = get_client_id(options_count, options);

	twitch_v5_game_list *games = twitch_v5_search_games(CLIENT_ID, name, false);
	if (games != NULL && games->count > 0) {
		for (int index = 0; index < games->count; index++) {
			twitch_v5_game *game = games->items[index];
			printf(
				"ID: %lld\n  Name: %s\n  Popularity: %d\n  Giantbomb ID: %lld\n",
				game->id,
				game->name,
				game->popularity,
				game->giantbomb_id);
		}
		twitch_v5_game_list_free(games);
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
	char *CLIENT_ID = get_client_id(options_count, options);

	twitch_v5_stream_list *streams = twitch_v5_search_all_streams(
		CLIENT_ID,
		query,
		none
	);

	if (streams != NULL && streams->count > 0) {
		printf("Streams found: %d\n", streams->count);
		for (int index = 0; index < streams->count; index++) {
			twitch_v5_stream *stream = streams->items[index];
			printf(
				"ID: %lld\n  Game: %s\n  Channel: %s\n	Status: %s\n	URL: %s\n",
				stream->id,
				stream->game,
				stream->channel->name,
				stream->channel->status,
				stream->channel->url
			);
		}
		twitch_v5_stream_list_free(streams);
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
	char *CLIENT_ID = get_client_id(options_count, options);

	// Find user by login name to get their user ID.
	twitch_v5_user *user = twitch_v5_get_user_by_username(CLIENT_ID, username);
	if (user == NULL) {
		fprintf(stderr, "Error: user with login '%s' not found.\n", username);
		return;
	}

	// Convert ID to string.
	char* user_id = malloc(64 * sizeof(char));
	sprintf(user_id, "%lld", user->id);

	// Get all user's follows.
	twitch_v5_follow_list *follows = twitch_v5_get_all_user_follows(
		CLIENT_ID,
		user_id,
		NULL,
		NULL
	);

	if (follows == NULL) {
		twitch_v5_user_free(user);
		fprintf(stderr, "Error: failed to get user's follows.\n");
		return;
	}

	// Get all streams from user's follows.
	char **channel_ids = (char **)pointer_array_map(
		(void **)follows->items,
		follows->count,
		&channel_id_from_follow
	);
	int streams_count = 0;
	twitch_v5_stream_list *streams = twitch_v5_get_all_streams(
		CLIENT_ID,
		follows->count,
		(const char **)channel_ids,
		NULL,
		NULL,
		NULL
	);

	// Print stream data.
	if (streams != NULL && streams->count > 0) {
		for (int idx = 0; idx < streams->count; idx++) {
			twitch_v5_stream *stream = streams->items[idx];
			printf(
				"ID: %lld\n  Game: %s\n  Channel: %s\n	Channel ID: %lld\n	Status: %s\n	URL: %s\n",
				stream->id,
				stream->game,
				stream->channel->name,
				stream->channel->id,
				stream->channel->status,
				stream->channel->url
			);
		}
		twitch_v5_stream_list_free(streams);
	}

	// Cleanup.
	twitch_v5_user_free(user);

	int channels_count = follows->count;
	twitch_v5_follow_list_free(follows);

	pointer_array_free(
		channels_count,
		(void **)channel_ids,
		(void(*)(void*))&free
	);
	free(CLIENT_ID);
}

/**
 * Prints top games.
 *
 * @param query Query string.
 * @param options_count Number of command line arguments.
 * @param options List of command line arguments.
 */
void get_top_games(const char *query, int options_count, const char **options) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *BEARER = get_bearer_token(options_count, options);

	int limit = atoi(query);
	if (limit <= 0) {
		fprintf(
			stderr,
			"Error: could not convert argument to positive integer number.\n"
		);
		exit(EXIT_FAILURE);
	}

	int size = 0, total = 0;
	twitch_helix_game_list *games = twitch_helix_get_all_top_games(
		CLIENT_ID,
		BEARER,
		limit
	);

	if (games != NULL && games->count > 0) {
		printf("Total: %d\n", games->count);
		for (int index = 0; index < games->count; index++) {
			twitch_helix_game *game = games->items[index];
			printf(
				"Game: %s\n  ID: %s\n  IGDB ID: %s\n  Art: %s\n\n",
				game->name,
				game->id,
				game->igdb_id,
				game->box_art_url
			);
		}
		twitch_helix_game_list_free(games);
	}

	free(CLIENT_ID);
	free(BEARER);
}

void get_channel_followers(
	const char *query,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *USER_TOKEN = get_user_token(options_count, options, 1);
	const char *usernames[1] = { query };

	twitch_helix_user_list *users = twitch_helix_get_users(
		CLIENT_ID,
		USER_TOKEN,
		1,
		usernames
	);
	if (users == NULL) {
		fprintf(stderr, "Error: failed to get user info\n");
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}
	if (users->count == 0) {
		fprintf(stderr, "Error: user not found\n");
		twitch_helix_user_list_free(users);
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}

	twitch_helix_user *user = users->items[0];

	char user_id[128] = { 0 };
	sprintf(user_id, "%lld", user->id);

	twitch_helix_follower_list *followers = twitch_helix_get_all_channel_followers(
		CLIENT_ID,
		USER_TOKEN,
		user_id,
		NULL,
		0
	);

	printf("Total followers: %d\n", followers->count);
	if (followers->count > 0) {
		for (int index = 0; index < followers->count; index++) {
			twitch_helix_follower *follower = followers->items[index];
			printf("%s%s",
				follower->user_name,
				(index < (followers->count - 1)) ? ", " : "\n"
			);
		}
		twitch_helix_follower_list_free(followers);
	}

	free(CLIENT_ID);
	free(USER_TOKEN);
	twitch_helix_user_list_free(users);
}

void get_channel_teams(
	const char *query,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *BEARER = get_bearer_token(options_count, options);

	const char *usernames[1] = { query };

	twitch_helix_user_list *users = twitch_helix_get_users(
		CLIENT_ID,
		BEARER,
		1,
		usernames
	);
	if (users == NULL) {
		fprintf(stderr, "Error: failed to get user info\n");
		free(CLIENT_ID);
		free(BEARER);
		return;
	}
	if (users->count == 0) {
		fprintf(stderr, "Error: user not found\n");
		twitch_helix_user_list_free(users);
		free(CLIENT_ID);
		free(BEARER);
		return;
	}

	twitch_helix_user *user = users->items[0];

	char user_id[128] = { 0 };
	sprintf(user_id, "%lld", user->id);

	twitch_helix_team_list *teams = twitch_helix_get_channel_teams(
		CLIENT_ID,
		BEARER,
		user_id
	);

	if (teams->count > 0) {
		printf("Total team count: %d\n", teams->count);
		for (int index = 0; index < teams->count; index++) {
			twitch_helix_team *team = teams->items[index];
			printf("%s\n	Info: %s\n", team->display_name, team->info);
		}
		twitch_helix_team_list_free(teams);
	}

	free(CLIENT_ID);
	free(BEARER);
	twitch_helix_user_list_free(users);
}

void get_channel_communities(
	const char *query,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *channel_id = find_channel_id(CLIENT_ID, query);

	if (channel_id == NULL) {
		printf("Channel '%s' not found\n", query);
		return;
	}

	twitch_v5_community_list *communities = twitch_v5_get_channel_communities(
		CLIENT_ID,
		channel_id
	);
	if (communities->count > 0) {
		for (int index = 0; index < communities->count; index++) {
			twitch_v5_community *community = communities->items[index];
			printf("%s\n	Name: %s\n	ID: %s\n	Summary: %s\n",
				community->display_name,
				community->name,
				community->id,
				community->summary
			);
		}
		twitch_v5_community_list_free(communities);
	}

	free(CLIENT_ID);
	free(channel_id);
}

void get_channel_videos(
	const char *query,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *channel_id = find_channel_id(CLIENT_ID, query);

	if (channel_id == NULL) {
		printf("Channel '%s' not found\n", query);
		return;
	}

	twitch_v5_video_list *videos = twitch_v5_get_all_channel_videos(
		CLIENT_ID, channel_id,
		NULL,
		NULL,
		NULL
	);
	if (videos->count > 0) {
		for (int index = 0; index < videos->count; index++) {
			twitch_v5_video *video = videos->items[index];
			printf(
				"%s\n	Title: %s\n  Game: %s\n  Description: %s\n",
				video->id, video->title, video->game, video->description
			);
		}
		twitch_v5_video_list_free(videos);
	}

	free(CLIENT_ID);
	free(channel_id);
}

void get_team(const char *query, int options_count, const char **options) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *BEARER = get_bearer_token(options_count, options);

	twitch_helix_team *team = twitch_helix_get_team(
		CLIENT_ID,
		BEARER,
		query,
		NULL
	);

	if (team != NULL) {
		printf(
			"Team: %s\n  Display Name: %s\n  Info:\n=======\n%s\n=======\n  Users:\n",
			team->name,
			team->display_name,
			team->info
		);
		if (team->users != NULL) {
			for (int index = 0; index < team->users->count; index++) {
				printf(
					"    %d: %s, Name: %s\n",
					index,
					team->users->items[index]->login,
					team->users->items[index]->name
				);
			}
		}
		twitch_helix_team_free(team);
	}

	free(CLIENT_ID);
	free(BEARER);
}

/**
 * Gets and prints the app access token.
 *
 * @param param Parameter string (optional)
 * @param options_count Number of provided options.
 * @param options List of option strings.
 */
void get_token(const char *param, int options_count, const char **options) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *CLIENT_SECRET = get_client_secret(options_count, options);
	//char *scope[] = { "user:read:email", "user:read:follows" };

	twitch_app_access_token *token = twitch_get_app_access_token(
		CLIENT_ID,
		CLIENT_SECRET
	);

	if (token != NULL) {
		printf(
			"Token:\n  access_token: %s\n  expires_in: %d\n  token_type: %s\n",
			token->token,
			token->expires_in,
			token->token_type
		);
		twitch_app_access_token_free(token);
	}

	free(CLIENT_ID);
	free(CLIENT_SECRET);
}

/**
 * Gets and prints username data.
 *
 * @param username Username to get user info for.
 * @param options_count Number of CLI arguments.
 * @param options List of arguments.
 */
void get_helix_user(
	const char *username,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *CLIENT_SECRET = get_client_secret(options_count, options);
	const char *usernames[1] = { username };

	twitch_app_access_token *token = twitch_get_app_access_token(
		CLIENT_ID,
		CLIENT_SECRET
	);
	if (token == NULL) {
		free(CLIENT_ID);
		free(CLIENT_SECRET);
		fprintf(stderr, "Error: failed to get auth token\n");
		return;
	}

	twitch_helix_user_list *users = twitch_helix_get_users(
		CLIENT_ID,
		token->token,
		1,
		usernames
	);

	if (users != NULL) {
		printf("Users: %d\n", users->count);
		for (int idx = 0; idx < users->count; idx++) {
			twitch_helix_user *user = users->items[idx];
			printf(
				"Username: %s\n  ID: %lld\n  Display Name: %s\n  Created At: %s\n  Type: %s\n  Description: %s\n",
				user->login,
				user->id,
				user->display_name,
				user->created_at,
				user->type,
				user->description
			);
		}
		twitch_helix_user_list_free(users);
	} else {
		fprintf(stderr, "Error: user with login '%s' not found\n", username);
	}

	free(CLIENT_ID);
	free(CLIENT_SECRET);
}

/**
 * Gets and prints list of follows from a given channel.
 *
 * @param username Username to get user info for.
 * @param options_count Number of CLI arguments.
 * @param options List of arguments.
 */
void get_helix_channel_follows(
	const char *username,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *USER_TOKEN = get_user_token(options_count, options, 1);
	const char *usernames[1] = { username };

	twitch_helix_user_list *users = twitch_helix_get_users(
		CLIENT_ID,
		USER_TOKEN,
		1,
		usernames
	);
	if (users != NULL) {
		if (users->count > 0) {
			twitch_helix_user *user = users->items[0];
			twitch_helix_channel_follow_list *follows =
				twitch_helix_get_all_channel_follows(
					CLIENT_ID,
					USER_TOKEN,
					user->id,
					0
				);
			if (follows != NULL) {
				printf("Follows: %d\n", follows->count);
				for (int idx = 0; idx < follows->count; idx++) {
					twitch_helix_channel_follow *follow = follows->items[idx];
					printf("Name: %s [Login: %s] (Id: %lld)\n",
						follow->broadcaster_name,
						follow->broadcaster_login,
						follow->broadcaster_id
					);
				}
				twitch_helix_channel_follow_list_free(follows);
			} else {
				fprintf(stderr, "Error: user with login '%s' not found\n", username);
			}
		}
		twitch_helix_user_list_free(users);
	} else {
		fprintf(stderr, "Error: user with login '%s' not found\n", username);
	}

	free(CLIENT_ID);
	free(USER_TOKEN);
}

/**
 * Gets and prints list of live follows from a given user using channel API.
 *
 * @param username Username to get user info for.
 * @param options_count Number of CLI arguments.
 * @param options List of arguments.
 */
void get_helix_live_channel_follows(
	const char *username,
	int options_count,
	const char **options
) {
	char *CLIENT_ID = get_client_id(options_count, options);
	char *USER_TOKEN = get_user_token(options_count, options, 1);
	const char *usernames[1] = { username };

	twitch_helix_user_list *users = twitch_helix_get_users(
		CLIENT_ID,
		USER_TOKEN,
		1,
		usernames
	);
	if (users == NULL) {
		fprintf(stderr, "Error: failed to get user info\n");
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}
	if (users->count == 0) {
		fprintf(stderr, "Error: user not found\n");
		twitch_helix_user_list_free(users);
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}

	twitch_helix_user *user = users->items[0];

	twitch_helix_channel_follow_list *follows = twitch_helix_get_all_channel_follows(
		CLIENT_ID,
		USER_TOKEN,
		user->id,
		0
	);
	if (follows == NULL) {
		fprintf(stderr, "Error: user with login '%s' not found\n", username);
		twitch_helix_user_list_free(users);
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}
	if (follows->count == 0) {
		fprintf(stderr, "No follows returned for user %s\n", user->login);
		twitch_helix_user_list_free(users);
		twitch_helix_channel_follow_list_free(follows);
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	}

	long long *user_ids = malloc(sizeof(long long) * follows->count);
	for (int idx = 0; idx < follows->count; idx++) {
		user_ids[idx] = follows->items[idx]->broadcaster_id;
	}
	twitch_helix_stream_list *streams = twitch_helix_get_all_streams(
		CLIENT_ID,
		USER_TOKEN,
		0,
		NULL,
		follows->count,
		user_ids,
		0,
		NULL
	);

	if (streams == NULL) {
		fprintf(stderr, "Error: failed to get/parse streams list\n");
		twitch_helix_user_list_free(users);
		twitch_helix_channel_follow_list_free(follows);
		free(CLIENT_ID);
		free(USER_TOKEN);
		return;
	} else {
		for (int idx = 0; idx < streams->count; idx++) {
			printf(
				"ID: %lld\n	Game: %s\n	Channel: %s\n  Channel ID: %d\n  Title: %s\n",
				streams->items[idx]->id,
				streams->items[idx]->game_name,
				streams->items[idx]->user_name,
				streams->items[idx]->user_id,
				streams->items[idx]->title
			);
		}
	}

	twitch_helix_stream_list_free(streams);
	twitch_helix_user_list_free(users);
	twitch_helix_channel_follow_list_free(follows);
	free(CLIENT_ID);
	free(USER_TOKEN);
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
		},
		{
			.command = top_games,
			.name = "top-games",
			.description = "Prints top N games on Twitch. Query must be an integer between 1 through 100.",
			.has_parameter = true,
			.handler = &get_top_games
		},
		{
			.command = channel_follows,
			.name = "followers",
			.description = "Gets all users that follow specific channel.",
			.has_parameter = true,
			.handler = &get_channel_followers
		},
		{
			.command = channel_teams,
			.name = "teams",
			.description = "Gets all teams of specific channel.",
			.has_parameter = true,
			.handler = &get_channel_teams
		},
		{
			.command = channel_communities,
			.name = "communities",
			.description = "Gets all communities of specific channel.",
			.has_parameter = true,
			.handler = &get_channel_communities
		},
		{
			.command = channel_videos,
			.name = "videos",
			.description = "Gets all videos of specific channel.",
			.has_parameter = true,
			.handler = &get_channel_videos
		},
		{
			.command = team,
			.name = "team",
			.description = "Gets team description and users.",
			.has_parameter = true,
			.handler = &get_team
		},
		{
			.command = token,
			.name = "token",
			.description = "Gets app access token.",
			.has_parameter = false,
			.handler = &get_token
		},
		{
			.command = helix_user,
			.name = "helix_user",
			.description = "Gets user info for specific login",
			.has_parameter = true,
			.handler = &get_helix_user
		},
		{
			.command = helix_channel_follows,
			.name = "helix_channel_follows",
			.description = "Gets channel's follows",
			.has_parameter = true,
			.handler = &get_helix_channel_follows
		},
		{
			.command = helix_live_channel_follows,
			.name = "helix_live_channel_follows",
			.description = "Gets channel's live follows",
			.has_parameter = true,
			.handler = &get_helix_live_channel_follows
		},
	};

	int commands_count = sizeof(commands)/sizeof(command_spec);

	// No command: just print usage and exit.
	if (argc < 2) {
		print_usage(commands_count, commands);
	}

	// If there's a command, try to match it with one of the known ones, and
	// execute it's handler.
	for (int cmd_idx = 0; cmd_idx < commands_count; cmd_idx++) {
		command_spec command = commands[cmd_idx];
		if (strcmp(argv[1], command.name) == 0) {
			if (command.handler != NULL) {
				// Don't forget to initialize the library!
				twitch_v5_init();

				// Handle the command. We currently only support one additional
				// argument/param.
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

