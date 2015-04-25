#include <pebble.h>
#include "main_menu.h"

#define players			0
#define ready			1
#define gameStatus		2
#define flag			3


static Window *window;
static TextLayer *top_text;
static TextLayer *middle_text;
static TextLayer *bottom_text;
static TextLayer *fpm_text;
Layer *window_layer;
bool black = false;
int players_waiting = 0;
DictionaryIterator *it2;

void go_black() {  
	text_layer_set_background_color(top_text, GColorBlack);
	text_layer_set_text_color(top_text, GColorWhite);
	text_layer_set_background_color(middle_text, GColorBlack);
	text_layer_set_text_color(middle_text, GColorWhite);
	text_layer_set_background_color(bottom_text, GColorBlack);
	text_layer_set_text_color(bottom_text, GColorWhite);
}

void go_white() {
	text_layer_set_background_color(top_text, GColorWhite);
	text_layer_set_text_color(top_text, GColorBlack);
	text_layer_set_background_color(middle_text, GColorWhite);
	text_layer_set_text_color(middle_text, GColorBlack);
	text_layer_set_background_color(bottom_text, GColorWhite);
	text_layer_set_text_color(bottom_text, GColorBlack);
}

void menu_handle_feedback(char *feed) {
	players_waiting = atoi(feed);
	if (black && players_waiting < 2) {
		black = false;
		go_white();
	}
}

void menu_change_info(char *info) {
	text_layer_set_text(bottom_text, info);
}

void middle_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (players_waiting > 1) {
		black = true;
		// 0 = start game, 1 = correct, 2 = failure
		dict_write_uint32(it2, flag, 0);
		app_message_outbox_begin(&it2);
		app_message_outbox_send();
		
		// GOBLACK
		go_black();
	}
}

void config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, middle_click_handler);
}


void menu_deinit() {
	text_layer_destroy(top_text);
	text_layer_destroy(middle_text);
	text_layer_destroy(bottom_text);
	text_layer_destroy(fpm_text);
	window_destroy(window);
}

void init_menu() {
	window = window_create();
	window_stack_push(window, true /* Animated */);
	window_set_click_config_provider(window, config_provider);
	window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	
	GFont top_font		= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_35));
	GFont middle_font	= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_40));
	GFont bottom_font	= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_20));  
	GFont fpm_font		= fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_18));  
	
	top_text	= text_layer_create(bounds);
	middle_text	= text_layer_create(bounds);
	bottom_text	= text_layer_create(bounds);
	fpm_text	= text_layer_create(bounds);
	text_layer_set_text(top_text, "COBBLE");
	text_layer_set_text(middle_text, "STONE");
	text_layer_set_text(bottom_text, "Pending Server..");
	text_layer_set_font(top_text, top_font);
	text_layer_set_font(middle_text, middle_font);
	text_layer_set_font(bottom_text, bottom_font);
	text_layer_set_font(fpm_text, fpm_font);
	text_layer_set_text_alignment(top_text, GTextAlignmentCenter);
	text_layer_set_text_alignment(middle_text, GTextAlignmentCenter);
	text_layer_set_text_alignment(bottom_text, GTextAlignmentCenter);
	text_layer_set_text_alignment(fpm_text, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(top_text));
	layer_add_child(window_layer, text_layer_get_layer(middle_text));
	layer_add_child(window_layer, text_layer_get_layer(bottom_text));
	layer_add_child(window_layer, text_layer_get_layer(fpm_text));
	
	GRect move_posy = (GRect) { .origin = { -15, 10 }, .size = { 180, 180 } };
	layer_set_frame(text_layer_get_layer(top_text), move_posy);
	
	GRect move_pos = (GRect) { .origin = { -15, 49 }, .size = { 180, 180 } };
	layer_set_frame(text_layer_get_layer(middle_text), move_pos);
	GRect move_pos2 = (GRect) { .origin = { -15, 105 }, .size = { 180, 180 } };
	layer_set_frame(text_layer_get_layer(bottom_text), move_pos2);
	GRect move_pos3 = (GRect) { .origin = { -15, 130 }, .size = { 180, 180 } };
	layer_set_frame(text_layer_get_layer(fpm_text), move_pos3);
}
