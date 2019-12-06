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
} twitch_art;

/**
 * Allocates and clears memory for new twitch_art struct.
 *
 * @return Pointer to allocated twitch_art struct.
 */
twitch_art *twitch_art_alloc();

/**
 * Frees the memory for given twitch_art struct and all it's properties.
 *
 * @param art Struct to deallocate.
 */
void twitch_art_free(twitch_art *art);

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
} twitch_user;

/**
 * Allocates and erases memory for new twitch_user instance
 *
 * @return Pointer to dynamically allocated twitch_user struct.
 */
twitch_user *twitch_user_alloc();

/**
 * Frees the memory allocated for twitch_user struct and all its properties.
 *
 * @param user Dynamically allocated twitch user data struct.
 */
void twitch_user_free(twitch_user *user);

typedef struct {
  int count;
  twitch_user **items;
} twitch_user_list;

/**
 * Allocates new twitch_user_list struct.
 *
 * @return Pointer to newly allocated list.
 */
twitch_user_list *twitch_user_list_alloc();

/**
 * Frees memory occupied by all twitch_user structs inside given list, then frees the list itself.
 *
 * @param list List to deallocate.
 */
void twitch_users_list_free(twitch_user_list *list);

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
} twitch_channel;

/**
 * Dynamically allocates and initializes twitch_channel struct. Use this to ensure all properties are set to NULL
 * by default.
 *
 * @return Pointer to new twitch_channel struct. You'll have to manually free it later using twitch_channel_free().
 */
twitch_channel *twitch_channel_alloc();

/**
 * Frees all propertes of twitch_channel struct, and the struct itself.
 *
 * @param channel Struct to deallocate.
 */
void twitch_channel_free(twitch_channel *channel);

typedef struct {
  int count;
  twitch_channel **items;
} twitch_channel_list;

/**
 * Allocates new twitch_channel_list struct.
 *
 * @return Pointer to newly allocated channel list struct.
 */
twitch_channel_list *twitch_channel_list_alloc();

/**
 * Frees memory occupied by given twitch_channel_list struct and it's items.
 *
 * @param list List to deallocate.
 */
void twitch_channel_list_free(twitch_channel_list *list);

/** Follow data **/

/**
 * User's follow info.
 */
typedef struct {
  char *created_at;
  int notifications;
  twitch_channel *channel;
} twitch_follow;

/**
 * Allocates new instance of twitch_follow struct.
 *
 * @return Pointer to newly allocated twitch_follow struct.
 */
twitch_follow *twitch_follow_alloc();

/**
 * Frees all the properties of twitch_follow struct, and the struct itself.
 *
 * @param follow. Data structure to deallocate.
 */
void twitch_follow_free(twitch_follow *follow);

typedef struct {
  int count;
  twitch_follow **items;
} twitch_follow_list;

/**
 * Allocates new twitch_follow_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_follow_list *twitch_follow_list_alloc();

/**
 * Helper function to release a dynamically allocated list of pointers to twitch_follow elements.
 * Goes through the array and calls twitch_follow_free on each element, and then frees the array itself.
 *
 * @param list List to free.
 */
void twitch_follows_list_free(twitch_follow_list *list);

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
  twitch_channel *channel;
  char *created_at;
  int delay;
  char *game;
  int is_playlist;
  int video_height;
  int viewers;
  twitch_art *preview;
} twitch_stream;

/**
 * Allocates and clears the memory for twitch_stream struct.
 *
 * @return Pointer to a newly created twitch_stream struct.
 */
twitch_stream *twitch_stream_alloc();

/**
 * Frees all properties of twitch_stream struct, and the struct itself.
 *
 * @param stream Data structure to deallocate.
 */
void twitch_stream_free(twitch_stream *stream);

typedef struct {
  int count;
  twitch_stream **items;
} twitch_stream_list;

/**
 * Allocates new instance of twitch_stream_list struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_stream_list *twitch_stream_list_alloc();

/**
 * Helper function to release dynamically allocated array of twitch_stream elements.
 * Goes through array and calls twitch_stream_free on each element, and frees the array itself afterwards.
 *
 * @param list Array of pointers to twitch_stream elements.
 */
void twitch_stream_list_free(twitch_stream_list *list);

/** Stream summary. **/

typedef struct {
  int channels;
  int viewers;
} twitch_summary;

/**
 * Creates new empty twitch_summary struct.
 *
 * @return Newly allocated twitch_summary instance.
 */
twitch_summary *twitch_summary_alloc();

/**
 * Releases memory allocated for twitch_summary struct.
 *
 * @param summary A struct to deallocate.
 */
void twitch_summary_free(twitch_summary *summary);

/** Featured streams. **/
typedef struct {
  char *image;
  int priority;
  int scheduled;
  int sponsored;
  char *text;
  char *title;
  twitch_stream *stream;
} twitch_featured_stream;

/**
 * Allocates memory for new featured stream struct.
 *
 * @return Pointer to newly allocated twitch_featured_stream struct.
 */
twitch_featured_stream *twitch_featured_stream_alloc();

/**
 * Recursively frees memory allocated for twitch_featured_stream struct and it's members.
 *
 * @param stream Struct to deallocate.
 */
void twitch_featured_stream_free(twitch_featured_stream *stream);

typedef struct {
  int count;
  twitch_featured_stream **items;
} twitch_featured_stream_list;

/**
 * Allocates twitch_featured_stream_list struct.
 *
 * @return Pointer to new structure.
 */
twitch_featured_stream_list *twitch_featured_stream_list_alloc();

/**
 * Frees memory allocated for given array of featured stream objects and for all objects inside it.
 *
 * @param list Pointer to the list holding featured stream objects.
 */
void twitch_featured_stream_list_free(twitch_featured_stream_list *list);

/** Game data **/

typedef struct {
  long long int id;
  long long int giantbomb_id;
  char *name;
  char *localized_name;
  char *locale;
  int popularity;
  twitch_art *box;
  twitch_art *logo;
} twitch_game;

/**
 * Allocates and clears memory for new twitch_game struct.
 *
 * @return Pointer to newly allocated twitch_game struct.
 */
twitch_game *twitch_game_alloc();

/**
 * Frees the memory allocated for given twitch_game instance and all it's properties.
 *
 * @param game Struct to deallocate.
 */
void twitch_game_free(twitch_game *game);

typedef struct {
  int count;
  twitch_game **items;
} twitch_game_list;

/**
 * Allocates a twitch_game_list struct.
 *
 * @return Pointer to newly allocated instance.
 */
twitch_game_list *twitch_game_list_alloc();

/**
 * Deallocates twitch_game_list struct with all it's content.
 *
 * @param list List to deallocate.
 */
void twitch_game_list_free(twitch_game_list *list);

/** Top games data **/

typedef struct {
  twitch_game *game;
  int channels;
  int viewers;
} twitch_top_game;

/**
 * Allocates and clears memory for new twitch_top_game struct.
 *
 * @return Pointer to newly allocated twitch_game struct.
 */
twitch_top_game *twitch_top_game_alloc();

/**
 * Frees memory allocated for given twitch_top_game struct.
 *
 * @param game Instance of twitch_top_game struct to deallocate.
 */
void twitch_top_game_free(twitch_top_game *game);

typedef struct {
  int count;
  twitch_top_game **items;
} twitch_top_game_list;

/**
 * Allocates new instance of twitch_top_game_list struct;
 *
 * @return Pointer to the new list.
 */
twitch_top_game_list *twitch_top_game_list_alloc();

/**
 * Deallocates an array of dynamically allocated twitch_top_game structs.
 *
 * @param count Number of items in the array.
 * @param list List to deallocate.
 */
void twitch_top_game_list_free(twitch_top_game_list *list);

/** Followers data **/

typedef struct {
  char *created_at;
  bool notifications;
  twitch_user *user;
} twitch_follower;

/**
 * Allocates and clears memory for twitch_follower struct.
 *
 * @return Pointer to newly allocated twitch_follower struct.
 */
twitch_follower *twitch_follower_alloc();

/**
 * Frees the memory allocated for twitch_follower struct and all its properties.
 *
 * @param follower Object to deallocate.
 */
void twitch_follower_free(twitch_follower *follower);

typedef struct {
  int count;
  twitch_follower **items;
} twitch_follower_list;

/**
 * Allocates twitch_follower_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_follower_list* twitch_follower_list_alloc();

/**
 * Deallocates an array of dynamically allocated twitch_follower structs.
 *
 * @param list List to deallocate.
 */
void twitch_follower_list_free(twitch_follower_list *list);

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
} twitch_community;

/**
 * Allocates and initializes new twitch_community struct.
 *
 * @return Pointer to newly allocated tiwtch_community struct.
 */
twitch_community *twitch_community_alloc();

/**
 * Frees memory allocated for twitch_community struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_community_free(twitch_community *community);

typedef struct {
  int count;
  twitch_community **items;
} twitch_community_list;

/**
 * Allocates new instance of twitch_community_list struct.
 *
 * @return Pointer to newly allocated twitch_community_list struct.
 */
twitch_community_list *twitch_community_list_alloc();

/**
 * Deallocates an instance of twitch_community_list and its content.
 *
 * @param list List to deallocate.
 */
void twitch_community_list_free(twitch_community_list *list);

/** Videos **/

typedef struct {
  char *chunked;
  char *high;
  char *low;
  char *medium;
  char *mobile;
} twitch_resolutions;

/**
 * Allocates and initializes new twitch_resolutions struct.
 *
 * @return Pointer to newly allocated twitch_resolutions struct.
 */
twitch_resolutions *twitch_resolutions_alloc();

/**
 * Frees memory allocated for twitch_resolutions struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_resolutions_free(twitch_resolutions *resolutions);

typedef struct {
  float chunked;
  float high;
  float low;
  float medium;
  float mobile;
} twitch_fps;

/**
 * Allocates and initializes new twitch_fps struct.
 *
 * @return Pointer to newly allocated twitch_fps struct.
 */
twitch_fps *twitch_fps_alloc();

/**
 * Frees memory allocated for twitch_fps struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_fps_free(twitch_fps *fps);

typedef struct {
  long long int id;
  char *display_name;
  char *name;
} twitch_channel_ref;

/**
 * Allocates and initializes new twitch_channel_ref struct.
 *
 * @return Pointer to newly allocated twitch_channel_ref struct.
 */
twitch_channel_ref *twitch_channel_ref_alloc();

/**
 * Frees memory allocated for twitch_channel_ref struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_channel_ref_free(twitch_channel_ref *ref);

typedef struct {
  char *type;
  char *url;
} twitch_thumbnail;

/**
 * Allocates memory for new instance of twitch_thumbnail struct;
 *
 * @return Pointer to newly allocated struct.
 */
twitch_thumbnail *twitch_thumbnail_alloc();

/**
 * Frees the memory occupied by twitch_thumbnail struct and it's properties.
 *
 * @param thumbnail Pointer to thumbnail struct to deallocate.
 */
void twitch_thumbnail_free(twitch_thumbnail *thumbnail);

typedef struct {
  size_t count;
  twitch_thumbnail **items;
} twitch_thumbnail_list;

/**
 * Allocates new twitch_thumbnail_list struct.
 *
 * @return Pointer to allocated memory.
 */
twitch_thumbnail_list *twitch_thumbnail_list_alloc();

/**
 * Frees the memory allocated for twitch_thumbnail_list struct and it's content.
 *
 * @param list List to deallocate.
 */
void twitch_thumbnail_list_free(twitch_thumbnail_list *list);

typedef struct {
  twitch_thumbnail_list* large;
  twitch_thumbnail_list* medium;
  twitch_thumbnail_list* small;
  twitch_thumbnail_list* template;
} twitch_video_thumbnails;

/**
 * Allocates new twitch_video_thumbnails struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_video_thumbnails *twitch_video_thumbnails_alloc();

/**
 * Frees the memory allocated for twitch_video_thumbnails struct and it's content.
 *
 * @param list List to deallocate.
 */
void twitch_video_thumbnails_free(twitch_video_thumbnails *thumbnails);

typedef struct {
  char *id;
  int broadcast_id;
  char *broadcast_type;
  twitch_channel_ref *channel;
  char *created_at;
  char *description;
  char *description_html;
  twitch_fps *fps;
  char *game;
  char *language;
  int length;
  twitch_art *preview;
  char *published_at;
  twitch_resolutions *resolutions;
  char *status;
  char *tag_list;
  twitch_video_thumbnails *thumbnails;
  char *title;
  char *url;
  char *viewable;
  char *viewable_at;
  int views;
} twitch_video;

/**
 * Allocates new twitch_video struct.
 *
 * @return Pointer to allocated struct.
 */
twitch_video *twitch_video_alloc();

/**
 * Frees the memory allocated for twitch_video struct and it's content.
 *
 * @param Struct to deallocate.
 */
void twitch_video_free(twitch_video *video);

typedef struct {
  int count;
  twitch_video **items;
} twitch_video_list;

/**
 * Allocates new twitch_video_list struct.
 *
 * @return Pointer to new struct.
 */
twitch_video_list *twitch_video_list_alloc();

/**
 * Frees memory occupied by array of twitch_video structs.
 *
 * @param list List to free.
 */
void twitch_video_list_free(twitch_video_list *list);

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
  twitch_channel_list *users;
} twitch_team;

/**
 * Allocates and clears memory for new twitch_team struct.
 *
 * @return Pointer to newly allocated twitch_team struct.
 */
twitch_team *twitch_team_alloc();

/**
 * Frees memory allocated for given twitch_team struct.
 *
 * @param team Struct to deallocate.
 */
void twitch_team_free(twitch_team *team);

typedef struct {
  int count;
  twitch_team **items;
} twitch_team_list;

/**
 * Allocates new twitch_team_list struct.
 *
 * @return Pointer to newly allocated twitch_team_list struct.
 */
twitch_team_list *twitch_team_list_alloc();

/**
 * Frees memory allocated for each twitch_team struct in given array, and the array itself
 *
 * @param list Array to deallocate.
 */
void twitch_team_list_free(twitch_team_list *list);

#endif

