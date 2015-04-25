#include "game_shape_memory.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_circle_small_white;
static GBitmap *s_res_diamond_small_white;
static GBitmap *s_res_square_small_white;
static GFont s_res_gothic_18_bold;
static ActionBarLayer *s_actionbarlayer_1;
static BitmapLayer *s_bitmaplayer_1;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_circle_small_white = gbitmap_create_with_resource(RESOURCE_ID_CIRCLE_SMALL_WHITE);
  s_res_diamond_small_white = gbitmap_create_with_resource(RESOURCE_ID_DIAMOND_SMALL_WHITE);
  s_res_square_small_white = gbitmap_create_with_resource(RESOURCE_ID_SQUARE_SMALL_WHITE);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorClear);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_circle_small_white);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_diamond_small_white);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_square_small_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(42, 45, 60, 60));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(20, 55, 100, 40));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "What was the 0th shape?");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  bitmap_layer_destroy(s_bitmaplayer_1);
  text_layer_destroy(s_textlayer_1);
  gbitmap_destroy(s_res_circle_small_white);
  gbitmap_destroy(s_res_diamond_small_white);
  gbitmap_destroy(s_res_square_small_white);
}
// END AUTO-GENERATED UI CODE

// ivars
static GBitmap *s_bmp_circle;
static GBitmap *s_bmp_diamond;
static GBitmap *s_bmp_square;
static int num_bitmaps;
static int *bitmap_sequence;
static int bitmap_sequence_count;
static int question_image_index;

static void click_config_provider(void *context);

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

void app_timer_callback(void *data) {
	static bool image_is_on_screen = false;
	if (image_is_on_screen) {
		// hide image
		image_is_on_screen = false;
		bitmap_layer_set_bitmap(s_bitmaplayer_1, NULL);
		app_timer_register(500 /* ms */, app_timer_callback, NULL);
	} else {
		if (bitmap_sequence_count < num_bitmaps) {
			// show next image
			image_is_on_screen = true;
			GBitmap *bitmap = bitmap_for_index(bitmap_sequence[bitmap_sequence_count++]);
			bitmap_layer_set_bitmap(s_bitmaplayer_1, bitmap);
			app_timer_register(500 /* ms */, app_timer_callback, NULL);
		} else {
			// pose question
			char *num_string;
			switch (question_image_index + 1) {
			case 1:
				num_string = "1st";
				break;
			case 2:
				num_string = "2nd";
				break;
			case 3:
				num_string = "3rd";
				break;
			default:
				num_string = "0th";
				snprintf(num_string, strlen(num_string) + 1,
						 "%1dth", question_image_index + 1);
				break;
			}
			char question_str[100];
			snprintf(question_str, strlen("What was the 0th shape?") + 1,
					 "What was the %s shape?", num_string);
			text_layer_set_text(s_textlayer_1, question_str);
			// enable inputs
			action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider);
			layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
		}
	}
}

static void win() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You win!");
}

static void lose() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You lose!");
}

// Click handling
static void animation_stopped_handler(struct Animation *animation, bool finished, void *context) {
	animation_destroy(animation);
}

static void handle_click(const int button) {
	const GBitmap *actual = bitmap_for_index(bitmap_sequence[question_image_index]);
	const GBitmap *guessed = bitmap_for_index(button);
	if (actual == guessed) {
		win();
	} else {
		lose();
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

static void init_bitmaps(void) {
	// hide text and inputs
	text_layer_set_text(s_textlayer_1, NULL);
	layer_remove_from_parent((Layer *)s_actionbarlayer_1);
	// init bitmaps
	s_bmp_circle	= gbitmap_create_with_resource(RESOURCE_ID_CIRCLE_WHITE);
	s_bmp_diamond	= gbitmap_create_with_resource(RESOURCE_ID_DIAMOND_WHITE);
	s_bmp_square	= gbitmap_create_with_resource(RESOURCE_ID_SQUARE_WHITE);
	// init bitmap layers
	num_bitmaps = 3 + rand() % 4; // rand in range [3, 6]
	bitmap_sequence_count = 0;
	question_image_index = rand() % num_bitmaps;
	// generate the sequence
	bitmap_sequence = (int *)calloc(num_bitmaps, sizeof(int));
	for (int i = 0; i < num_bitmaps; i++) {
		bitmap_sequence[i] = rand() % 3;
	}
	// start the callbacks
	app_timer_register(500 /* ms */, app_timer_callback, NULL);
}

static void deinit_bitmaps(void) {
	free(bitmap_sequence);
	gbitmap_destroy(s_bmp_circle);
	gbitmap_destroy(s_bmp_diamond);
	gbitmap_destroy(s_bmp_square);
}

static void handle_window_unload(Window* window) {
	destroy_ui();
	deinit_bitmaps();
}

void show_game_shape_memory(void) {
	initialise_ui();
	init_bitmaps();
	window_set_window_handlers(s_window, (WindowHandlers) {
		.unload = handle_window_unload,
	});
//	window_set_click_config_provider(s_window, click_config_provider);
	window_stack_push(s_window, true);
}

void hide_game_shape_memory(void) {
	window_stack_remove(s_window, true);
}

