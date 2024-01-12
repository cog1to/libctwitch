#include <curl/curl.h>
#include <ctwitch/ctwitch.h>

void twitch_helix_init() {
  // Global cURL init.
  curl_global_init(CURL_GLOBAL_ALL);
} 
