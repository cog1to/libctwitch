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

/** Follows **/

typedef struct {
	long long from_id;
	char *from_name;
	long long to_id;
	char *to_name;
	char *followed_at;
} twitch_helix_follow;

/**
 * Creates new instance of twitch_helix_follow structure.
 *
 * @return Pointer to newly allocated twitch_helix_follow;
 */
twitch_helix_follow *twitch_helix_follow_alloc();

/**
 * Frees the memory occupied by twitch_helix_follow structure.
 *
 * @param follow Follow structure to deallocate.
 */
void twitch_helix_follow_free(twitch_helix_follow *follow);

typedef struct {
	int count;
	twitch_helix_follow **items;
} twitch_helix_follow_list;

/**
 * Allocates new twitch_helix_follow_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_helix_follow_list *twitch_helix_follow_list_alloc();

/**
 * Frees memory occupied by all twitch_helix_follow structs inside given list,
 * then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_follow_list_free(twitch_helix_follow_list *list);

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

#endif
