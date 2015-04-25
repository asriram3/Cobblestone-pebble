#pragma once
#include <pebble.h>

extern const int kPebbleScreenWidth;
	// compensates for the status bar.
extern const int kPebbleScreenHeight;

#define APPMESG_PLAYERS		0
#define APPMESG_READY		1
#define APPMESG_GAME_STATUS	2
#define APPMESG_FLAG		3

extern void win(void);
extern void lose(void);
extern void out_of_time(void);

	// Returns an int in [min, max)
int rand_in_range(int min, int max);

void game_init();
void game_deinit();
