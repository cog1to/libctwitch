/**
 * String data types and utilities.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <stdlib.h>

/**
 * Dynamically resizable string.
 */
typedef struct string {
  char *ptr;
  size_t len;
} string_t;

/**
 * Returns new dynamically allocated instance of string_t struct.
 *
 * @return New instance of string_t.
 */
string_t *string_init();

/**
 * Returns new string_t struct with specified value.
 *
 * @param value. Initial string's value.
 *
 * @return New instance of string_t holding specified value.
 */
string_t *string_init_with_value(const char* value);

/**
 * Appends a string to given dynamic string.
 *
 * @param ptr String to append.
 * @param size Size of the string to append. Can be less than actual ptr's size if you want to append only part of the string.
 * @param s String to append to.
 */
void string_append(const char* ptr, size_t size, string_t *s);

/**
 * Appends a formatted string to given dynamic string.
 *
 * @param s String to append to.
 * @param fmt String format to append, like in sprintf functions.
 */
void string_append_format(string_t *s, const char *fmt, ...);

/**
 * Frees dynamic string.
 *
 * @param s String to free.
 */
void string_free(string_t *s);

/**
 * Copies given string to another memory location.
 *
 * @param src String to copy.
 *
 * @return Pointer to the new string instance.
 */
char *immutable_string_copy(const char *src);

/**
 * Returns a string produced by joining a list of strings.
 *
 * @param counst Number of strings to join.
 * @param strings Array of strings to join.
 * @param delimiter String to insert in between each 2 strings.
 *
 * @return Dynamic string instance containing joined string.
 */
string_t *string_joined(int count, const char **strings, const char *delimiter);

#endif

