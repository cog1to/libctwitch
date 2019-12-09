#include <curl/curl.h>
#include "ctwitch.h"

void twitch_v5_init() {
  // Global cURL init.
  curl_global_init(CURL_GLOBAL_ALL);
} 
