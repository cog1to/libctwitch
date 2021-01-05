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

#endif
