#pragma once
#include <pebble.h>

extern const int kPebbleScreenWidth;
	// compensates for the status bar.
extern const int kPebbleScreenHeight;

BitmapLayer *bitmap_layer_rotate_right(BitmapLayer *bitmapLayer);

	// Returns an int in [min, max)
int rand_in_range(int min, int max);

void game_init();
void game_deinit();
