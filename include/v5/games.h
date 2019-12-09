/**
 * Twitch V5 API - Games
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_V5_GAMES
#define _H_TWITCH_V5_GAMES

#include <stdio.h>
#include <stdint.h>
#include "data.h"

/**
 * Returns one page of games data sorted by number of current viewers, most popular first.
 *
 * @param client_id Twitch API client ID.
 * @param limit Page size.
 * @param offset List offset.
 * @param total Will return total number of items in the remote top games list.
 *
 * @return An array of pointers to dynamically allocated twitch_top_game structs. You will
 *   have to deallocate it, either manually or using twitch_v5_top_game_list_free() function.
 */
twitch_v5_top_game_list *twitch_v5_get_top_games(const char *client_id, int limit, int offset, int *total);

/**
 * Returns the full list of games on Twitch, sorted by number of current viewers, most popular
 *   first.
 *
 * @param client_id Twitch API client ID.
 *
 * @return An array of pointers to dynamically allocated twitch_top_game structs. You will
 *   have to deallocate it, either manually or using twitch_v5_top_game_list_free() function.
 */
twitch_v5_top_game_list *twitch_v5_get_all_top_games(const char *client_id);

#endif

