#include <stdlib.h>

#include <ctwitch/common.h>

#include "utils/datagen.h"

/** Utils **/

twitch_string_list *twitch_string_list_alloc() {
	GENERIC_ALLOC(twitch_string_list)
}

void twitch_string_list_free(twitch_string_list *list) {
	if (list == NULL) {
		return;
	}

	if (list->items != NULL) {
		for (int idx = 0; idx < list->count; idx++) {
			free(list->items[idx]);
		}
		free(list->items);
	}

	free(list);
}

