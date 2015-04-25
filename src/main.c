#include <pebble.h>
#include "common.h"


static void init() {
	srand(time(NULL)); // seed pRNG
	
	game_init();
}

static void deinit() {
	game_deinit();
}

int main() {
	init();
	app_event_loop();
	deinit();
}
