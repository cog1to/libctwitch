#include <stdlib.h>

#include "utils/data/data.h"

/** Helper functions **/

void free_if_not_empty(void *ptr, void(*deallocator)(void *)) {
  if (ptr != NULL) {
    (*deallocator)(ptr);
  }
}
