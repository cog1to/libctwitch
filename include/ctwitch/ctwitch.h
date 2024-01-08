#ifndef _H_TWITCH_INIT
#define _H_TWITCH_INIT

/**
 * Initializes twitch library. Currently it just calls curl_global_init(), so
 * you can skip this and call cURL init directly.
 */
void twitch_v5_init();

#endif

