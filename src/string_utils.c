#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "string_utils.h"

string_t *string_init() {
  string_t *s = malloc(sizeof(string_t));
  s->len = 0;
  s->ptr = malloc(s->len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
  return s;
}

void string_append(const char *ptr, size_t size, string_t *s) {
  size_t new_len = s->len + size;
  s->ptr = realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr + s->len, ptr, size);
  s->ptr[new_len] = '\0';
  s->len = new_len;
}

void string_append_format(string_t *s, const char *fmt, ...) {
  va_list args;
  char buffer[1024];

  // Write formatted string to the buffer.
  va_start(args, fmt);
  int result = sprintf(buffer, fmt, args);
  va_end(args);

  string_append(buffer, strlen(buffer), s);
}

void string_free(string_t *s) {
  free(s->ptr);
  free(s);
}

string_t *string_init_with_value(const char *value) {
  string_t *s = string_init();
  string_append(value, strlen(value), s);
  return s;
}

char *immutable_string_copy(const char *value) {
  char *str = malloc(strlen(value) + 1);
  strcpy(str, value);
  return str;
}

string_t *string_joined(int count, const char **strings, const char *delimiter) {
  if (count == 0) {
    return string_init();
  }

  if (count == 1) {
    return string_init_with_value(strings[0]);
  }

  string_t *output = string_init();
  for (int index = 0; index < count - 1; index++) {
    string_append((void *)strings[index], strlen(strings[index]), output);
    string_append(",", 1, output);
  }

  string_append((void *)strings[count - 1], strlen(strings[count - 1]), output);
  return output;
}
