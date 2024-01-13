/**
 * Twitch Helix API - Videos
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_TWITCH_HELIX_VIDEOS
#define _H_TWITCH_HELIX_VIDEOS

#include <stdlib.h>

#include <ctwitch/common.h>
#include <ctwitch/helix/data.h>

/**
 * Downloads one page of videos list matching given search parameters.
 *
 * user_id, game_id and ids parameters are mutially exclusive. You must specify
 * only one of them, i.e. you can search for either videos for specific user,
 * specific game/category, or specific videos.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @param error Error holder struct.
 * @param user_id ID of user from whom to fetch videos.
 * @param game_id ID of the game or category.
 * @param id_count Number of video IDs in `ids` param.
 * @param ids List of video IDS.
 * @param language Language filter. Can be NULL, or a language code, like "en"
 * @param period Time period filter. Can be NULL or one of "all", "day",
 * "month", "week".
 * @param sort Sorting parameter. Can be NULL or one of "time", "trending" or
 * "views".
 * @param type Video type filter. Can be NULL or one of "all", "archive",
 * "highlight" or "upload".
 * @param first Page size. Must be between 1 and 100, inclusive.
 * @param after Pagination cursor.
 * @param next Will return a cursor to the next page.
 *
 * @return List of videos in a twitch_helix_video_list struct.
 * */
twitch_helix_video_list *twitch_helix_get_videos(
	const char *client_id,
	const char *token,
	twitch_error *error,
	const char *user_id,
	const char *game_id,
	int id_count,
	const char **ids,
	const char *language,
	const char *period,
	const char *sort,
	const char *type,
	int first,
	const char *after,
	char **next
);

/**
 * Downloads full list of videos matching given search parameters.
 *
 * user_id, game_id and ids parameters are mutially exclusive. You must specify
 * only one of them, i.e. you can search for either videos for specific user,
 * specific game/category, or specific videos.
 *
 * @param client_id Twitch API client ID.
 * @param token Bearer token.
 * @oaram error Error holder struct.
 * @param user_id ID of user from whom to fetch videos.
 * @param game_id ID of the game or category.
 * @param id_count Number of video IDs in `ids` param.
 * @param ids List of video IDS.
 * @param language Language filter. Can be NULL, or a language code, like "en"
 * @param period Time period filter. Can be NULL or one of "all", "day",
 * "month", "week".
 * @param sort Sorting parameter. Can be NULL or one of "time", "trending" or
 * "views".
 * @param type Video type filter. Can be NULL or one of "all", "archive",
 * "highlight" or "upload".
 * @param limit Max number of videos. If 0, the method will try to return all
 * available videos matching given parameters.
 *
 * @return List of videos in a twitch_helix_video_list struct.
 */
twitch_helix_video_list *twitch_helix_get_all_videos(
	const char *client_id,
	const char *token,
	twitch_error *error,
	const char *user_id,
	const char *game_id,
	int id_count,
	const char **ids,
	const char *language,
	const char *period,
	const char *sort,
	const char *type,
	int limit
);

#endif
