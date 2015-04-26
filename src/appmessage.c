#include <pebble.h>
#include "appmessage.h"


#define players			0
#define ready			1
#define gameStatus		2
#define flag			3


static void appmesg_send_uint(uint32_t num_to_send);


void appmesg_send_win() {
	appmesg_send_uint(3);
}

void appmesg_send_death() {
	appmesg_send_uint(4);
}

static void appmesg_send_uint(uint32_t num_to_send) {
	DictionaryIterator *iter = NULL;
	app_message_outbox_begin(&iter);
	if (iter) {
		dict_write_uint32(iter, flag, num_to_send);
		app_message_outbox_send();
	}
}
