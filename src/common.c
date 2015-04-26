#include <pebble.h>
#include "common.h"
#include "game_ddr.h"


const int kPebbleScreenWidth	= 144;
const int kPebbleScreenHeight	= 152;

int rand_in_range(int min, int max) {
	return min + (int)((max - min) * (1.0 * rand() / RAND_MAX));
}

Layer *g_time_layer;
int g_time_remaining;
const int MAX_TIME = 25000; // in ms
static AppTimer *time_layer_redraw_timer;

static void time_layer_request_redraw(void *data) {
	g_time_remaining -= 100;
	layer_mark_dirty(g_time_layer);
	// check for death
	if (g_time_remaining < 0) {
		out_of_time();
		app_timer_cancel(time_layer_redraw_timer);
	} else {
		time_layer_redraw_timer = app_timer_register(100, time_layer_request_redraw, NULL);
	}
}

	// only call this to get the start and end values for the animation
static GRect time_layer_frame_for_time(int timeRemaining) {
	const int height = (int)(138.0 * timeRemaining / MAX_TIME);
	return GRect(0, 148 - height, 10, height);
}

static void draw_time_layer(struct Layer *layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	GRect fillRect = time_layer_frame_for_time(g_time_remaining); //*/
	graphics_fill_rect(ctx, fillRect, 0, GCornerNone);
}

void set_time_remaining(int newTimeRemaining) {
	g_time_remaining = newTimeRemaining;
	app_timer_cancel(time_layer_redraw_timer);
	time_layer_redraw_timer = app_timer_register(100, time_layer_request_redraw, NULL);
}

void game_init() {
	g_time_layer = layer_create(GRect(0, 10, 10, 138));
	layer_set_update_proc(g_time_layer, draw_time_layer);
}

void game_deinit() {
	app_timer_cancel(time_layer_redraw_timer);
	layer_remove_from_parent(g_time_layer);
	layer_destroy(g_time_layer);
}
