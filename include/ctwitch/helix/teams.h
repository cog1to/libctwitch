/**
 * Twitch Helix API - Teams
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_TEAMS
#define _H_TWITCH_HELIX_TEAMS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctwitch/helix/data.h>

/**
 * Gets details for one team identified by its name or ID. Specify either ID or
 * name. If ID is provided, name parameter will be ignored.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @param id ID of the team to fetch.
 * @param name Name of the team to fetch.
 *
 * @return Team details inside twitch_team struct.
 */
twitch_helix_team *twitch_helix_get_team(
	const char *client_id,
	const char *token,
	const char *name,
	const char *id
);

#endif
