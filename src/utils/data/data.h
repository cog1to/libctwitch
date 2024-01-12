/**
 * Data-related utility functions.
 *
 * @author Alexander Rogachev
 * @version 0.1
 */

#ifndef _H_DATA_UTILS
#define _H_DATA_UTILS

void free_if_not_empty(void *ptr, void(*deallocator)(void *));

#endif
