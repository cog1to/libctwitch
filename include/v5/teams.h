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
 * @param size Will return total nubmer of downloaded items.
 *
 * @return List of pointers to twitch_team structs with downloaded team data. Note that 'users' property will be NULL for all teams. To get list of users for a team, use twitch_v5_get_team method.
 */
twitch_team **twitch_v5_get_teams(const char *client_id, int limit, int offset, int* size);

/**
 * Returns a full list of all registered teams.
 *
 * @param client_id Twitch API client ID.
 * @param size Will return total number of items in the returned list.
 *
 * @return List of pointers to twitch_team structs with downloaded team data. Note that 'users' property will be NULL for all teams. To get list of users for a team, use twitch_v5_get_team method.
 */
twitch_team **twitch_v5_get_all_teams(const char *client_id, int *size);

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
