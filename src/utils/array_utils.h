/**
 * Array helper functions.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_ARRAY_UTILS
#define _H_ARRAY_UTILS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Performs map operation over the array of pointers by applying getter function to each
 * element of the array.
 *
 * @param src Array of pointers to source elements.
 * @param src_count Number of items in the source array.
 * @param getter Function to apply to each element. Must return another pointer.
 *
 * @return Array of pointers of mapped values.
 */
void **pointer_array_map(void **src, size_t src_count, void *(*getter)(void *));

/**
 * Frees given array of pointers using specified deinit function.
 *
 * This is a convenience function to recursively deallocate complex and dynamically allocated
 * data structures. For example, for an array of twitch_follow structs, we can use this function
 * to recursively deallocate all the memory allocated for it by invoking something like this:
 *
 * pointer_array_free(count, (void **)follows, (void(*)(void*))&twitch_follow_free)
 *
 * @param count Number of items in the array.
 * @param src Array of pointers to the items.
 * @param deinit Function to apply to each item to properly free it.
 */
void pointer_array_free(int count, void **src, void(*deinit)(void *));

#endif

