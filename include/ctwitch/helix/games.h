/**
 * Twitch Helix API - Games
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_GAMES
#define _H_TWITCH_HELIX_GAMES

#include <stdio.h>
#include <stdint.h>
#include <ctwitch/helix/data.h>

/**
 * Returns one page of games data sorted by number of current viewers, most
 * popular first.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @param first Page size. Min 1, max 100.
 * @param after Cursor value to get next page. Can be NULL.
 *
 * @return An array of pointers to dynamically allocated twitch_helix_game
 * structs. You will have to deallocate it, either manually or using
 * twitch_helix_game_list_free() function.
 */
twitch_helix_game_list *twitch_helix_get_top_games(
	const char *client_id,
	const char *token,
	int first,
	const char *after
);

/**
 * Returns list of games data sorted by number of current viewers, most
 * popular first.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @param limit Number of top games to load.
 *
 * @return An array of pointers to dynamically allocated twitch_helix_game
 * structs. You will have to deallocate it, either manually or using
 * twitch_helix_game_list_free() function.
 */
twitch_helix_game_list *twitch_helix_get_all_top_games(
	const char *client_id,
	const char *token,
	int limit
);

#endif

