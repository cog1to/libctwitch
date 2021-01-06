#ifndef _H_HELIX_DATA
#define _H_HELIX_DATA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/** String list **/
typedef struct {
  int count;
  char **items;
} twitch_string_list;

/** Auth token **/
typedef struct {
  char *token;
  int expires_in;
  char *token_type;
  twitch_string_list scope;
} twitch_helix_auth_token;

/**
 * Allocates and clears memory for new twitch_helix_auth_token struct.
 *
 * @return Pointer to allocated twitch_helix_auth_token struct.
 */
twitch_helix_auth_token *twitch_helix_auth_token_alloc();

/**
 * Frees the memory for given twitch_helix_auth_token struct and all it's properties.
 *
 * @param art Struct to deallocate.
 */
void twitch_helix_auth_token_free(twitch_helix_auth_token *token);

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
 * Frees the memory allocated for twitch_helix_user struct and all its properties.
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
 * Frees memory occupied by all twitch_helix_user structs inside given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_helix_user_list_free(twitch_helix_user_list *list);

#endif
