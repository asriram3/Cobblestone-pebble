#include "game_ddr.h"
#include "common.h"
#include "linked_list.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
}

static void destroy_ui(void) {
  window_destroy(s_window);
}
// END AUTO-GENERATED UI CODE


// ivars
static Node arrows_up, arrows_right, arrows_down;

// forward declares
static void loop_main();
static void create_random_arrow();
static bool arrow_is_clickable(BitmapLayer *arrow);
	// click handlers
static void click_handler_up(		ClickRecognizerRef recognizer, void *context);
static void click_handler_select(	ClickRecognizerRef recognizer, void *context);
static void click_handler_down(		ClickRecognizerRef recognizer, void *context);


static void loop_main() {
	
}

static void create_random_arrow() {
	
}

static bool arrow_is_clickable(BitmapLayer *arrow) {
	Layer *layer		= bitmap_layer_get_layer(arrow);
	GRect frame			= layer_get_frame(layer);
	int16_t right_edge	= frame.origin.x + frame.size.w;
	return kPebbleScreenWidth - 15 <= right_edge && right_edge < kPebbleScreenWidth;
}


// Click handling
static void click_handler_up(ClickRecognizerRef recognizer, void *context) {
	
}

static void click_handler_select(ClickRecognizerRef recognizer, void *context) {
	
}

static void click_handler_down(ClickRecognizerRef recognizer, void *context) {
	
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP,		click_handler_up);
	window_single_click_subscribe(BUTTON_ID_SELECT,	click_handler_select);
	window_single_click_subscribe(BUTTON_ID_DOWN,	click_handler_down);
}


// Default stuff
static void handle_window_unload(Window* window) {
	destroy_ui();
}

void show_game_ddr(void) {
	initialise_ui();
	
	arrows_up		= ll_create(NULL);
	arrows_right	= ll_create(NULL);
	arrows_down		= ll_create(NULL);
	
	window_set_window_handlers(s_window, (WindowHandlers) {
		.unload = handle_window_unload,
	});
	window_set_click_config_provider(s_window, click_config_provider);
	window_stack_push(s_window, true);
}

void hide_game_ddr(void) {
	ll_destroy(arrows_up);
	ll_destroy(arrows_right);
	ll_destroy(arrows_down);
	
	window_stack_remove(s_window, true);
}
