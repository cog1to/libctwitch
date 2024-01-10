#ifndef _H_HELIX_DATA
#define _H_HELIX_DATA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** User data **/

/**
 * Twitch User info. All properties are dynamically allocated.
 */
typedef struct {
	long long id;
	char *display_name;
	char *login;
	char *type;
	char *description;
	char *profile_image_url;
	char *offline_image_url;
	int view_count;
	char *created_at;
} twitch_helix_user;

/**
 * Allocates and erases memory for new twitch_helix_user instance
 *
 * @return Pointer to dynamically allocated twitch_helix_user struct.
 */
twitch_helix_user *twitch_helix_user_alloc();

/**
 * Frees the memory allocated for twitch_helix_user struct and all its
 * properties.
 *
 * @param user Dynamically allocated twitch user data struct.
 */
void twitch_helix_user_free(twitch_helix_user *user);

typedef struct {
	int count;
	twitch_helix_user **items;
} twitch_helix_user_list;

/**
 * Allocates new twitch_helix_user_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_user_list *twitch_helix_user_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_user structs inside given list,
 * then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_user_list_free(twitch_helix_user_list *list);

/** Channel follows **/

typedef struct {
	long long broadcaster_id;
	char *broadcaster_login;
	char *broadcaster_name;
	char *followed_at;
} twitch_helix_channel_follow;

/**
 * Creates new instance of twitch_helix_channel_follow structure.
 *
 * @return Pointer to newly allocated twitch_helix_channel_follow;
 */
twitch_helix_channel_follow *twitch_helix_channel_follow_alloc();

/**
 * Frees the memory occupied by twitch_helix_channel_follow structure.
 *
 * @param follow Follow structure to deallocate.
 */
void twitch_helix_channel_follow_free(twitch_helix_channel_follow *follow);

typedef struct {
	int count;
	twitch_helix_channel_follow **items;
} twitch_helix_channel_follow_list;

/**
 * Allocates new twitch_helix_channel_follow_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_channel_follow_list *twitch_helix_channel_follow_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_channel_follow structs inside
 * given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_channel_follow_list_free(
	twitch_helix_channel_follow_list *list
);

/** Streams **/

typedef struct {
	long long id;
	long long user_id;
	char *user_name;
	long long game_id;
	char *game_name;
	char *type;
	char *title;
	int viewer_count;
	char *started_at;
	char *language;
	char *thumbnail_url;
} twitch_helix_stream;

/**
 * Creates new instance of twitch_helix_stream structure.
 *
 * @return Pointer to newly allocated twitch_helix_stream;
 */
twitch_helix_stream *twitch_helix_stream_alloc();

/**
 * Frees the memory occupied by twitch_helix_stream structure.
 *
 * @param stream Stream structure to deallocate.
 */
void twitch_helix_stream_free(twitch_helix_stream *stream);

typedef struct {
	int count;
	twitch_helix_stream **items;
} twitch_helix_stream_list;

/**
 * Allocates new twitch_helix_stream_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_stream_list *twitch_helix_stream_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_stream structs inside given list,
 * then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_stream_list_free(twitch_helix_stream_list *list);

/** Games **/

typedef struct {
	char *id;
	char *igdb_id;
	char *name;
	char *box_art_url;
} twitch_helix_game;

/**
 * Allocates and clears memory for new twitch_helix_game struct.
 *
 * @return Pointer to newly allocated twitch_helix_game struct.
 */
twitch_helix_game *twitch_helix_game_alloc();

/**
 * Frees the memory allocated for given twitch_helix_game instance and all its
 * properties.
 *
 * @param game Struct to deallocate.
 */
void twitch_helix_game_free(twitch_helix_game *game);

typedef struct {
	int count;
	twitch_helix_game **items;
} twitch_helix_game_list;

/**
 * Allocates a twitch_helix_game_list struct.
 *
 * @return Pointer to newly allocated instance.
 */
twitch_helix_game_list *twitch_helix_game_list_alloc();

/**
 * Deallocates twitch_helix_game_list struct with all it's content.
 *
 * @param list List to deallocate.
 */
void twitch_helix_game_list_free(twitch_helix_game_list *list);

/** Teams **/

typedef struct {
	char *id;
	char *name;
	char *login;
} twitch_helix_team_member;

/**
 * Allocates and clears memory for new twitch_helix_team_member struct.
 *
 * @return Pointer to newly allocated twitch_helix_team_member struct.
 */
twitch_helix_team_member *twitch_helix_team_member_alloc();

/**
 * Frees memory allocated for given twitch_helix_team_member struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_helix_team_member_free(twitch_helix_team_member *user);

typedef struct {
	int count;
	twitch_helix_team_member **items;
} twitch_helix_team_member_list;

/**
 * Allocates new twitch_helix_channel_follow_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_team_member_list *twitch_helix_team_member_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_channel_follow structs inside
 * given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_team_member_list_free(
	twitch_helix_team_member_list *list
);

typedef struct {
	char *id;
	char *background;
	char *banner;
	char *created_at;
	char *updated_at;
	char *info;
	char *name;
	char *display_name;
	char *thumbnail;
	twitch_helix_team_member_list *users;
} twitch_helix_team;

/**
 * Allocates and clears memory for new twitch_helix_team struct.
 *
 * @return Pointer to newly allocated twitch_helix_team struct.
 */
twitch_helix_team *twitch_helix_team_alloc();

/**
 * Frees memory allocated for given twitch_helix_team struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_helix_team_free(twitch_helix_team *team);

typedef struct {
	int count;
	twitch_helix_team **items;
} twitch_helix_team_list;

/**
 * Allocates new twitch_helix_channel_follow_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_team_list *twitch_helix_team_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_channel_follow structs inside
 * given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_team_list_free(
	twitch_helix_team_list *list
);

/** Followers **/

typedef struct {
	char *user_id;
	char *user_name;
	char *user_login;
	char *followed_at;
} twitch_helix_follower;

/**
 * Allocates and clears memory for twitch_helix_follower struct.
 *
 * @return Pointer to newly allocated twitch_helix_follower struct.
 */
twitch_helix_follower *twitch_helix_follower_alloc();

/**
 * Frees the memory allocated for twitch_helix_follower struct and all its
 * properties.
 *
 * @param follower Object to deallocate.
 */
void twitch_helix_follower_free(twitch_helix_follower *follower);

typedef struct {
	int count;
	twitch_helix_follower **items;
} twitch_helix_follower_list;

/**
 * Allocates twitch_helix_follower_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_helix_follower_list* twitch_helix_follower_list_alloc();

/**
 * Deallocates an array of dynamically allocated twitch_helix_follower structs.
 *
 * @param list List to deallocate.
 */
void twitch_helix_follower_list_free(twitch_helix_follower_list *list);

#endif