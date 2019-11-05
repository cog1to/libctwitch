#ifndef _DATA_H
#define _DATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** User data **/

/**
 * Twitch User info. All properties are dynamically allocated.
 */
typedef struct {
  char *id;
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

/**
 * Frees memory occupied by all twitch_user structs inside given array, then frees the array itself.
 *
 * @param list Array of pointers to dynamically allocated twitch_user objects.
 * @param count Number of elements in the array.
 */
void twitch_users_list_free(twitch_user **list, int count);

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
  int mature;
  int partner;
  char *profile_banner;
  char *profile_banner_background_color;
  char *updated_at;
  char *url;
  char *video_banner;
  int views;
  char *broadcaster_type;
  char *broadcaster_software;
  int private_video;
  int privacy_options_enabled;
} twitch_channel;

/**
 * Dynamically allocates and initializes twitch_channel struct. Use this to ensure all properties are set to NULL
 * by default.
 *
 * @return Pointer to new twitch_channel struct. You'll have to manually free it later using twitch_channel_free().
 */
twitch_channel *twitch_channel_alloc();

/**
 * Dynamically allocates and initialized twitch_channel struct with given data values.
 *
 * @param id Channel ID.
 * @param game Game played on the channel.
 * @param name Twitch's name of the channel, i.e. "dyingcamel".
 * @param status Channel's status message.
 * @param display_name Channel's display name, i.e. "DyingCamel".
 *
 * @return Pointer to new twitch_channel struct. You'll have to manually free it later using twitch_channel_free().
 */
twitch_channel *twitch_channel_init(long long id, char *game, char *name, char *status, char *display_name);

/**
 * Frees all propertes of twitch_channel struct, and the struct itself.
 *
 * @param channel Struct to deallocate.
 */
void twitch_channel_free(twitch_channel *channel);

/**
 * Frees memory allocated for each twitch_channel struct inside the given array, and the array itself.
 *
 * @param count Number of items in the list.
 * @param list The list to deallocate.
 */
void twitch_channel_list_free(int count, twitch_channel **list);

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
 * Frees all the properties of twitch_follow struct, and the struct itself.
 *
 * @param follow. Data structure to deallocate.
 */
void twitch_follow_free(twitch_follow *follow);

/**
 * Allocates a new twitch_follow struct and initializes it with given data.
 *
 * @param created_at Created At date string.
 * @param notifications Flag indicating whether user has notifications enabled for the given channel.
 * @param channel Channel that was followed. When specified, the follow struct will assume ownership of this object
 *      and will try to free it when freeing itself.
 *
 * @return Initialized twitch_follow data struct.
 */
twitch_follow *twitch_follow_init(char *created_at, int notifications, twitch_channel *channel);

/**
 * Helper function to release a dynamically allocated list of pointers to twitch_follow elements.
 * Goes through the array and calls twitch_follow_free on each element, and then frees the array itself.
 *
 * @param list Array to free.
 * @param count Number of elements in the array.
 */
void twitch_follows_list_free(twitch_follow **list, int count);

/** Stream data **/

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
  // TODO: Add preview data.
} twitch_stream;

/**
 * Allocates and clears the memory for twitch_stream struct.
 *
 * @return Pointer to a newly created twitch_stream struct.
 */
twitch_stream *twitch_stream_alloc();

/**
 * @param id Stream ID.
 * @param average_fps Stream's average FPS count.
 * @param channel Stream's channel info.
 * @param created_at Created At date string
 * @param delay Chat delay.
 * @param game Name of the game played.
 * @param is_playlist Flag indicating whether stream is in Playlist mode, i.e. replaying previously recorded streams.
 * @param video_height Output video stream's height in pixels.
 * @param viewers Current number of viewers.
 *
 * @return Pointer to a newly allocated and initialized twitch_stream struct.
 */
twitch_stream *twitch_stream_init(long long int id, int average_fps, twitch_channel *channel, char *created_at, int delay, char *game, int is_playlist, int video_height, int viewers);

/**
 * Frees all properties of twitch_stream struct, and the struct itself.
 *
 * @param stream Data structure to deallocate.
 */
void twitch_stream_free(twitch_stream *stream);

/**
 * Helper function to release dynamically allocated array of twitch_stream elements.
 * Goes through array and calls twitch_stream_free on each element, and frees the array itself afterwards.
 *
 * @param list Array of pointers to twitch_stream elements.
 * @param count List of elements in the array.
 */
void twitch_streams_list_free(twitch_stream **list, int count);

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

/**
 * Frees memory allocated for given array of featured stream objects and for all objects inside it.
 *
 * @param count Number of elements in the array.
 * @param streams Pointer to the array holding featured stream objects.
 */
void twitch_featured_stream_list_free(int count, twitch_featured_stream **streams);

#endif

