#include <stdio.h>

/**
 * Code generation macros.
 */

#define GENERIC_ALLOC(T) \
  T *instance = calloc(1, sizeof(T)); \
  if (instance == NULL) { \
    fprintf(stderr, "Failed to allocate memory for " #T "");\
    exit(EXIT_FAILURE); \
  } \
  return instance;

#define FREE(prop) \
  if (prop != NULL) { \
    free(prop); \
  }

#define FREE_CUSTOM(prop, deinit) \
  if (prop != NULL) { \
    deinit(prop); \
  }

#define GENERIC_V5_LIST(entity) \
  twitch_v5_##entity##_list *twitch_v5_##entity##_list_alloc() { \
    GENERIC_ALLOC(twitch_v5_##entity##_list) \
  } \
  void twitch_v5_##entity##_list_free(twitch_v5_##entity##_list *list) { \
    pointer_array_free(list->count, (void **)list->items, (void(*)(void*))&twitch_v5_##entity##_free); \
    free(list); \
  }

#define GENERIC_HELIX_LIST(entity) \
  twitch_helix_##entity##_list *twitch_helix_##entity##_list_alloc() { \
    GENERIC_ALLOC(twitch_helix_##entity##_list) \
  } \
  void twitch_helix_##entity##_list_free(twitch_helix_##entity##_list *list) { \
    pointer_array_free(list->count, (void **)list->items, (void(*)(void*))&twitch_helix_##entity##_free); \
    free(list); \
  }
