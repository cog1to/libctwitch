/**
 * Twitch V5 API - Teams
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_V5_TEAMS
#define _H_TWITCH_V5_TEAMS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "data.h"

/**
 * Returns one page of team data.
 *
 * @param client_id Twitch API client ID.
 * @param limit Page size.
 * @param offset List offset.
 *
 * @return Pointer to twitch_team_list struct containing downloaded teams data.
 * */
twitch_team_list *twitch_v5_get_teams(const char *client_id, int limit, int offset);

/**
 * Returns a full list of all registered teams.
 *
 * @param client_id Twitch API client ID.
 *
 * @return Team list.
 * */
twitch_team_list *twitch_v5_get_all_teams(const char *client_id);

/**
 * Gets details for one team identified by it's name.
 *
 * @param client_id Twitch API client ID.
 * @param name Name of the team to fetch.
 *
 * @return Team details inside twitch_team struct.
 */
twitch_team *twitch_v5_get_team(const char *client_id, const char *name);

#endif
