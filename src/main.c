#include <pebble.h>
#include "common.h"
#include "game_ddr.h"


static void init() {
	srand(time(NULL)); // seed pRNG
	
	game_init();
	
	show_game_ddr();
}

static void deinit() {
	game_deinit();
}

int main() {
	init();
	app_event_loop();
	deinit();
}
