#ifndef _DATA_H
#define _DATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** Art data **/

typedef struct {
  char *large;
  char *medium;
  char *small;
  char *template;
} twitch_v5_art;

/**
 * Allocates and clears memory for new twitch_v5_art struct.
 *
 * @return Pointer to allocated twitch_v5_art struct.
 */
twitch_v5_art *twitch_v5_art_alloc();

/**
 * Frees the memory for given twitch_v5_art struct and all it's properties.
 *
 * @param art Struct to deallocate.
 */
void twitch_v5_art_free(twitch_v5_art *art);

/** User data **/

/**
 * Twitch User info. All properties are dynamically allocated.
 */
typedef struct {
  long long id;
  char *display_name;
  char *name;
  char *type;
  char *bio;
  char *created_at;
  char *updated_at;
  char *logo;
} twitch_v5_user;

/**
 * Allocates and erases memory for new twitch_v5_user instance
 *
 * @return Pointer to dynamically allocated twitch_v5_user struct.
 */
twitch_v5_user *twitch_v5_user_alloc();

/**
 * Frees the memory allocated for twitch_v5_user struct and all its properties.
 *
 * @param user Dynamically allocated twitch user data struct.
 */
void twitch_v5_user_free(twitch_v5_user *user);

typedef struct {
  int count;
  twitch_v5_user **items;
} twitch_v5_user_list;

/**
 * Allocates new twitch_v5_user_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_v5_user_list *twitch_v5_user_list_alloc();

/**
 * Frees memory occupied by all twitch_v5_user structs inside given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_v5_users_list_free(twitch_v5_user_list *list);

/** Channel data **/

/**
 * Twitch channel info. All string properties are dynamically allocated.
 */
typedef struct {
  long long int id;
  char *game;
  char *name;
  char *status;
  char *display_name;
  char *broadcaster_language;
  char *created_at;
  int followers;
  char *language;
  char *logo;
  bool mature;
  bool partner;
  char *profile_banner;
  char *profile_banner_background_color;
  char *updated_at;
  char *url;
  char *video_banner;
  int views;
  char *broadcaster_type;
  char *broadcaster_software;
  bool private_video;
  bool privacy_options_enabled;
} twitch_v5_channel;

/**
 * Dynamically allocates and initializes twitch_v5_channel struct. Use this to ensure all properties are set to NULL
 * by default.
 *
 * @return Pointer to new twitch_v5_channel struct. You'll have to manually free it later using twitch_v5_channel_free().
 */
twitch_v5_channel *twitch_v5_channel_alloc();

/**
 * Frees all propertes of twitch_v5_channel struct, and the struct itself.
 *
 * @param channel Struct to deallocate.
 */
void twitch_v5_channel_free(twitch_v5_channel *channel);

typedef struct {
  int count;
  twitch_v5_channel **items;
} twitch_v5_channel_list;

/**
 * Allocates new twitch_v5_channel_list struct.
 *
 * @return Pointer to newly allocated channel list struct.
 */
twitch_v5_channel_list *twitch_v5_channel_list_alloc();

/**
 * Frees memory occupied by given twitch_v5_channel_list struct and it's items.
 *
 * @param list List to deallocate.
 */
void twitch_v5_channel_list_free(twitch_v5_channel_list *list);

/** Follow data **/

/**
 * User's follow info.
 */
typedef struct {
  char *created_at;
  int notifications;
  twitch_v5_channel *channel;
} twitch_v5_follow;

/**
 * Allocates new instance of twitch_v5_follow struct.
 *
 * @return Pointer to newly allocated twitch_v5_follow struct.
 */
twitch_v5_follow *twitch_v5_follow_alloc();

/**
 * Frees all the properties of twitch_v5_follow struct, and the struct itself.
 *
 * @param follow. Data structure to deallocate.
 */
void twitch_v5_follow_free(twitch_v5_follow *follow);

typedef struct {
  int count;
  twitch_v5_follow **items;
} twitch_v5_follow_list;

/**
 * Allocates new twitch_v5_follow_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_v5_follow_list *twitch_v5_follow_list_alloc();

/**
 * Helper function to release a dynamically allocated list of pointers to twitch_v5_follow elements.
 * Goes through the array and calls twitch_v5_follow_free on each element, and then frees the array itself.
 *
 * @param list List to free.
 */
void twitch_v5_follow_list_free(twitch_v5_follow_list *list);

/** Stream data **/

typedef enum {
  none,
  hls,
  rtmp
} stream_filter;

/**
 * Live stream data.
 */
typedef struct {
  long long int id;
  double average_fps;
  twitch_v5_channel *channel;
  char *created_at;
  int delay;
  char *game;
  int is_playlist;
  int video_height;
  int viewers;
  twitch_v5_art *preview;
} twitch_v5_stream;

/**
 * Allocates and clears the memory for twitch_v5_stream struct.
 *
 * @return Pointer to a newly created twitch_v5_stream struct.
 */
twitch_v5_stream *twitch_v5_stream_alloc();

/**
 * Frees all properties of twitch_v5_stream struct, and the struct itself.
 *
 * @param stream Data structure to deallocate.
 */
void twitch_v5_stream_free(twitch_v5_stream *stream);

typedef struct {
  int count;
  twitch_v5_stream **items;
} twitch_v5_stream_list;

/**
 * Allocates new instance of twitch_v5_stream_list struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_v5_stream_list *twitch_v5_stream_list_alloc();

/**
 * Helper function to release dynamically allocated array of twitch_v5_stream elements.
 * Goes through array and calls twitch_v5_stream_free on each element, and frees the array itself afterwards.
 *
 * @param list Array of pointers to twitch_v5_stream elements.
 */
void twitch_v5_stream_list_free(twitch_v5_stream_list *list);

/** Stream summary. **/

typedef struct {
  int channels;
  int viewers;
} twitch_v5_summary;

/**
 * Creates new empty twitch_v5_summary struct.
 *
 * @return Newly allocated twitch_v5_summary instance.
 */
twitch_v5_summary *twitch_v5_summary_alloc();

/**
 * Releases memory allocated for twitch_v5_summary struct.
 *
 * @param summary A struct to deallocate.
 */
void twitch_v5_summary_free(twitch_v5_summary *summary);

/** Featured streams. **/
typedef struct {
  char *image;
  int priority;
  int scheduled;
  int sponsored;
  char *text;
  char *title;
  twitch_v5_stream *stream;
} twitch_v5_featured_stream;

/**
 * Allocates memory for new featured stream struct.
 *
 * @return Pointer to newly allocated twitch_v5_featured_stream struct.
 */
twitch_v5_featured_stream *twitch_v5_featured_stream_alloc();

/**
 * Recursively frees memory allocated for twitch_v5_featured_stream struct and it's members.
 *
 * @param stream Struct to deallocate.
 */
void twitch_v5_featured_stream_free(twitch_v5_featured_stream *stream);

typedef struct {
  int count;
  twitch_v5_featured_stream **items;
} twitch_v5_featured_stream_list;

/**
 * Allocates twitch_v5_featured_stream_list struct.
 *
 * @return Pointer to new structure.
 */
twitch_v5_featured_stream_list *twitch_v5_featured_stream_list_alloc();

/**
 * Frees memory allocated for given array of featured stream objects and for all objects inside it.
 *
 * @param list Pointer to the list holding featured stream objects.
 */
void twitch_v5_featured_stream_list_free(twitch_v5_featured_stream_list *list);

/** Game data **/

typedef struct {
  long long int id;
  long long int giantbomb_id;
  char *name;
  char *localized_name;
  char *locale;
  int popularity;
  twitch_v5_art *box;
  twitch_v5_art *logo;
} twitch_v5_game;

/**
 * Allocates and clears memory for new twitch_v5_game struct.
 *
 * @return Pointer to newly allocated twitch_v5_game struct.
 */
twitch_v5_game *twitch_v5_game_alloc();

/**
 * Frees the memory allocated for given twitch_v5_game instance and all it's properties.
 *
 * @param game Struct to deallocate.
 */
void twitch_v5_game_free(twitch_v5_game *game);

typedef struct {
  int count;
  twitch_v5_game **items;
} twitch_v5_game_list;

/**
 * Allocates a twitch_v5_game_list struct.
 *
 * @return Pointer to newly allocated instance.
 */
twitch_v5_game_list *twitch_v5_game_list_alloc();

/**
 * Deallocates twitch_v5_game_list struct with all it's content.
 *
 * @param list List to deallocate.
 */
void twitch_v5_game_list_free(twitch_v5_game_list *list);

/** Top games data **/

typedef struct {
  twitch_v5_game *game;
  int channels;
  int viewers;
} twitch_v5_top_game;

/**
 * Allocates and clears memory for new twitch_v5_top_game struct.
 *
 * @return Pointer to newly allocated twitch_v5_game struct.
 */
twitch_v5_top_game *twitch_v5_top_game_alloc();

/**
 * Frees memory allocated for given twitch_v5_top_game struct.
 *
 * @param game Instance of twitch_v5_top_game struct to deallocate.
 */
void twitch_v5_top_game_free(twitch_v5_top_game *game);

typedef struct {
  int count;
  twitch_v5_top_game **items;
} twitch_v5_top_game_list;

/**
 * Allocates new instance of twitch_v5_top_game_list struct;
 *
 * @return Pointer to the new list.
 */
twitch_v5_top_game_list *twitch_v5_top_game_list_alloc();

/**
 * Deallocates an array of dynamically allocated twitch_v5_top_game structs.
 *
 * @param count Number of items in the array.
 * @param list List to deallocate.
 */
void twitch_v5_top_game_list_free(twitch_v5_top_game_list *list);

/** Followers data **/

typedef struct {
  char *created_at;
  bool notifications;
  twitch_v5_user *user;
} twitch_v5_follower;

/**
 * Allocates and clears memory for twitch_v5_follower struct.
 *
 * @return Pointer to newly allocated twitch_v5_follower struct.
 */
twitch_v5_follower *twitch_v5_follower_alloc();

/**
 * Frees the memory allocated for twitch_v5_follower struct and all its properties.
 *
 * @param follower Object to deallocate.
 */
void twitch_v5_follower_free(twitch_v5_follower *follower);

typedef struct {
  int count;
  twitch_v5_follower **items;
} twitch_v5_follower_list;

/**
 * Allocates twitch_v5_follower_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_v5_follower_list* twitch_v5_follower_list_alloc();

/**
 * Deallocates an array of dynamically allocated twitch_v5_follower structs.
 *
 * @param list List to deallocate.
 */
void twitch_v5_follower_list_free(twitch_v5_follower_list *list);

/** Communities **/

typedef struct {
  char *id;
  long long int owner_id;
  char *name;
  char *display_name;
  char *avatar_image_url;
  char *cover_image_url;
  char *description;
  char *description_html;
  char *rules;
  char *rules_html;
  char *language;
  char *summary;
} twitch_v5_community;

/**
 * Allocates and initializes new twitch_v5_community struct.
 *
 * @return Pointer to newly allocated tiwtch_community struct.
 */
twitch_v5_community *twitch_v5_community_alloc();

/**
 * Frees memory allocated for twitch_v5_community struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_v5_community_free(twitch_v5_community *community);

typedef struct {
  int count;
  twitch_v5_community **items;
} twitch_v5_community_list;

/**
 * Allocates new instance of twitch_v5_community_list struct.
 *
 * @return Pointer to newly allocated twitch_v5_community_list struct.
 */
twitch_v5_community_list *twitch_v5_community_list_alloc();

/**
 * Deallocates an instance of twitch_v5_community_list and its content.
 *
 * @param list List to deallocate.
 */
void twitch_v5_community_list_free(twitch_v5_community_list *list);

/** Videos **/

typedef struct {
  char *chunked;
  char *high;
  char *low;
  char *medium;
  char *mobile;
} twitch_v5_resolutions;

/**
 * Allocates and initializes new twitch_v5_resolutions struct.
 *
 * @return Pointer to newly allocated twitch_v5_resolutions struct.
 */
twitch_v5_resolutions *twitch_v5_resolutions_alloc();

/**
 * Frees memory allocated for twitch_v5_resolutions struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_v5_resolutions_free(twitch_v5_resolutions *resolutions);

typedef struct {
  float chunked;
  float high;
  float low;
  float medium;
  float mobile;
} twitch_v5_fps;

/**
 * Allocates and initializes new twitch_v5_fps struct.
 *
 * @return Pointer to newly allocated twitch_v5_fps struct.
 */
twitch_v5_fps *twitch_v5_fps_alloc();

/**
 * Frees memory allocated for twitch_v5_fps struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_v5_fps_free(twitch_v5_fps *fps);

typedef struct {
  long long int id;
  char *display_name;
  char *name;
} twitch_v5_channel_ref;

/**
 * Allocates and initializes new twitch_v5_channel_ref struct.
 *
 * @return Pointer to newly allocated twitch_v5_channel_ref struct.
 */
twitch_v5_channel_ref *twitch_v5_channel_ref_alloc();

/**
 * Frees memory allocated for twitch_v5_channel_ref struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_v5_channel_ref_free(twitch_v5_channel_ref *ref);

typedef struct {
  char *type;
  char *url;
} twitch_v5_thumbnail;

/**
 * Allocates memory for new instance of twitch_v5_thumbnail struct;
 *
 * @return Pointer to newly allocated struct.
 */
twitch_v5_thumbnail *twitch_v5_thumbnail_alloc();

/**
 * Frees the memory occupied by twitch_v5_thumbnail struct and it's properties.
 *
 * @param thumbnail Pointer to thumbnail struct to deallocate.
 */
void twitch_v5_thumbnail_free(twitch_v5_thumbnail *thumbnail);

typedef struct {
  size_t count;
  twitch_v5_thumbnail **items;
} twitch_v5_thumbnail_list;

/**
 * Allocates new twitch_v5_thumbnail_list struct.
 *
 * @return Pointer to allocated memory.
 */
twitch_v5_thumbnail_list *twitch_v5_thumbnail_list_alloc();

/**
 * Frees the memory allocated for twitch_v5_thumbnail_list struct and it's content.
 *
 * @param list List to deallocate.
 */
void twitch_v5_thumbnail_list_free(twitch_v5_thumbnail_list *list);

typedef struct {
  twitch_v5_thumbnail_list* large;
  twitch_v5_thumbnail_list* medium;
  twitch_v5_thumbnail_list* small;
  twitch_v5_thumbnail_list* template;
} twitch_v5_video_thumbnails;

/**
 * Allocates new twitch_v5_video_thumbnails struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_v5_video_thumbnails *twitch_v5_video_thumbnails_alloc();

/**
 * Frees the memory allocated for twitch_v5_video_thumbnails struct and it's content.
 *
 * @param list List to deallocate.
 */
void twitch_v5_video_thumbnails_free(twitch_v5_video_thumbnails *thumbnails);

typedef struct {
  char *id;
  int broadcast_id;
  char *broadcast_type;
  twitch_v5_channel_ref *channel;
  char *created_at;
  char *description;
  char *description_html;
  twitch_v5_fps *fps;
  char *game;
  char *language;
  int length;
  twitch_v5_art *preview;
  char *published_at;
  twitch_v5_resolutions *resolutions;
  char *status;
  char *tag_list;
  twitch_v5_video_thumbnails *thumbnails;
  char *title;
  char *url;
  char *viewable;
  char *viewable_at;
  int views;
} twitch_v5_video;

/**
 * Allocates new twitch_v5_video struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_v5_video *twitch_v5_video_alloc();

/**
 * Frees the memory allocated for twitch_v5_video struct and it's content.
 *
 * @param Struct to deallocate.
 */
void twitch_v5_video_free(twitch_v5_video *video);

typedef struct {
  int count;
  twitch_v5_video **items;
} twitch_v5_video_list;

/**
 * Allocates new twitch_v5_video_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_v5_video_list *twitch_v5_video_list_alloc();

/**
 * Frees memory occupied by array of twitch_v5_video structs.
 *
 * @param list List to free.
 */
void twitch_v5_video_list_free(twitch_v5_video_list *list);

/** Teams **/

typedef struct {
  long long int id;
  char *background;
  char *banner;
  char *created_at;
  char *display_name;
  char *info;
  char *logo;
  char *name;
  char *updated_at;
  twitch_v5_channel_list *users;
} twitch_v5_team;

/**
 * Allocates and clears memory for new twitch_v5_team struct.
 *
 * @return Pointer to newly allocated twitch_v5_team struct.
 */
twitch_v5_team *twitch_v5_team_alloc();

/**
 * Frees memory allocated for given twitch_v5_team struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_v5_team_free(twitch_v5_team *team);

typedef struct {
  int count;
  twitch_v5_team **items;
} twitch_v5_team_list;

/**
 * Allocates new twitch_v5_team_list struct.
 *
 * @return Pointer to newly allocated twitch_v5_team_list struct.
 */
twitch_v5_team_list *twitch_v5_team_list_alloc();

/**
 * Frees memory allocated for each twitch_v5_team struct in given array, and the array itself
 *
 * @param list Array to deallocate.
 */
void twitch_v5_team_list_free(twitch_v5_team_list *list);

#endif

