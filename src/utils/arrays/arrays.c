#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void **pointer_array_map(
	void **src,
	size_t src_count,
	void *(*getter)(void *)
) {
  void **dest = malloc(sizeof(void *) * src_count);

  for (int index = 0; index < src_count; index++) {
    void *element = (*getter)(src[index]);
    dest[index] = element;
  }

  return dest;
}

void pointer_array_free(int count, void **src, void (*deinit)(void *)) {
  for (int index = 0; index < count; index++) {
    (*deinit)(src[index]);
  }

  free(src);
}
