#include "game_ddr.h"
#include "common.h"
//#include "linked_list.h"
//#include "appmessage.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_circle_small_white;
static GBitmap *s_res_diamond_small_white;
static GBitmap *s_res_square_small_white;
static ActionBarLayer *s_actionbarlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_circle_small_white = gbitmap_create_with_resource(RESOURCE_ID_CIRCLE_SMALL_WHITE);
  s_res_diamond_small_white = gbitmap_create_with_resource(RESOURCE_ID_DIAMOND_SMALL_WHITE);
  s_res_square_small_white = gbitmap_create_with_resource(RESOURCE_ID_SQUARE_SMALL_WHITE);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorClear);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_circle_small_white);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_diamond_small_white);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_square_small_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_circle_small_white);
  gbitmap_destroy(s_res_diamond_small_white);
  gbitmap_destroy(s_res_square_small_white);
}
// END AUTO-GENERATED UI CODE


// ivars
static GBitmap *s_bmp_circle;
static GBitmap *s_bmp_diamond;
static GBitmap *s_bmp_square;
static int num_layers;
static BitmapLayer **bitmap_layers;
static int *bitmap_answers;
static int bitmap_answer_index;

// bitmaps
static GBitmap *bitmap_for_index(const int index) {
	switch (index) {
	case 0:
		return s_bmp_circle;
	case 1:
		return s_bmp_diamond;
	case 2:
		return s_bmp_square;
	default:
		return NULL;
	}
}

// Click handling
static void animation_stopped_handler(struct Animation *animation, bool finished, void *context) {
	// remove the layer
	layer_remove_from_parent((Layer *)context);
	animation_destroy(animation);
}

static void handle_click(const int button) {
	const GBitmap *actual = bitmap_layer_get_bitmap(bitmap_layers[bitmap_answer_index]);
	const GBitmap *guessed = bitmap_for_index(button);
	if (actual != guessed) {
		lose();
		return;
	}
	// slide the shape off screen
	GRect offscreen_frame = layer_get_frame((Layer *)bitmap_layers[bitmap_answer_index]);
	offscreen_frame.origin.y = -offscreen_frame.size.h;
	PropertyAnimation *anim = property_animation_create_layer_frame(
		(Layer *)bitmap_layers[bitmap_answer_index], NULL, &offscreen_frame);
	AnimationHandlers handlers = {
		.started = NULL,
		.stopped = animation_stopped_handler
	};
	animation_set_handlers((Animation *)anim, handlers, bitmap_layers[bitmap_answer_index]);
	animation_schedule((Animation *)anim);
	
	bitmap_answer_index--;
	if (bitmap_answer_index < 0) {
		win();
	}
}
static void click_handler_up(ClickRecognizerRef recognizer, void *context) {
	handle_click(0);
}

static void click_handler_select(ClickRecognizerRef recognizer, void *context) {
	handle_click(1);
}

static void click_handler_down(ClickRecognizerRef recognizer, void *context) {
	handle_click(2);
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP,		click_handler_up);
	window_single_click_subscribe(BUTTON_ID_SELECT,	click_handler_select);
	window_single_click_subscribe(BUTTON_ID_DOWN,	click_handler_down);
}

static void handle_window_load(Window* window){
	// init bitmaps
	s_bmp_circle	= gbitmap_create_with_resource(RESOURCE_ID_CIRCLE_WHITE);
	s_bmp_diamond	= gbitmap_create_with_resource(RESOURCE_ID_DIAMOND_WHITE);
	s_bmp_square	= gbitmap_create_with_resource(RESOURCE_ID_SQUARE_WHITE);
	// init bitmap layers
	num_layers = 4 + rand() % 4; // rand in range [4, 7]
	bitmap_answer_index = num_layers - 1;
	bitmap_layers = (BitmapLayer **)calloc(num_layers, sizeof(BitmapLayer *));
	bitmap_answers = (int *)calloc(num_layers, sizeof(int));
	for (int i = 0; i < num_layers; i++) {
		// start at y = 66 and move up by 5 pixels for each additional layer
		const int layer_y = 66 - (8 * i);
		bitmap_layers[i] = bitmap_layer_create(GRect(42, layer_y, 60, 60));
		bitmap_layer_set_background_color(bitmap_layers[i], GColorClear);
		// choose a random bitmap
		bitmap_answers[i] = rand() % 3;//rand_in_range(0, 3);
		bitmap_layer_set_bitmap(bitmap_layers[i], bitmap_for_index(bitmap_answers[i]));
		layer_add_child(window_get_root_layer(s_window), (Layer *)bitmap_layers[i]);
	}
	// action bar
	//action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider);
	
	// add time remaining layer
	layer_add_child(window_get_root_layer(s_window), g_time_layer);
}


static void init_bitmaps(void) {
	
}

static void deinit_bitmaps(void) {
	free(bitmap_answers);
	for (int i = 0; i < num_layers; i++) {
		bitmap_layer_destroy(bitmap_layers[i]);
	}
	free(bitmap_layers);
	gbitmap_destroy(s_bmp_circle);
	gbitmap_destroy(s_bmp_diamond);
	gbitmap_destroy(s_bmp_square);
}

static void handle_window_unload(Window* window) {
	destroy_ui();
	deinit_bitmaps();
}

void show_game_ddr(void) {
	initialise_ui();
	init_bitmaps();
	window_set_window_handlers(s_window, (WindowHandlers) {
		.load = handle_window_load,
		.unload = handle_window_unload,
	});
	window_set_click_config_provider(s_window, click_config_provider);
	window_stack_push(s_window, true);
}

void hide_game_ddr(void) {
	window_stack_remove(s_window, true);
}
