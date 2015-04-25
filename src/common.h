#pragma once

extern const int kPebbleScreenWidth;
	// compensates for the status bar.
extern const int kPebbleScreenHeight;

BitmapLayer *bitmap_layer_rotate_right(BitmapLayer *bitmapLayer);

void game_init();
void game_deinit();
