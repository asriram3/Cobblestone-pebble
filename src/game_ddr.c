#include "game_ddr.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_arrow_right_white;
static GBitmap *s_res_arrow_up_white;
static GBitmap *s_res_arrow_down_white;
static BitmapLayer *s_bitmaplayer_1;
static BitmapLayer *s_bitmaplayer_2;
static BitmapLayer *s_bitmaplayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
  
  s_res_arrow_right_white = gbitmap_create_with_resource(RESOURCE_ID_ARROW_RIGHT_WHITE);
  s_res_arrow_up_white = gbitmap_create_with_resource(RESOURCE_ID_ARROW_UP_WHITE);
  s_res_arrow_down_white = gbitmap_create_with_resource(RESOURCE_ID_ARROW_DOWN_WHITE);
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(114, 61, 30, 30));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_arrow_right_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
  
  // s_bitmaplayer_2
  s_bitmaplayer_2 = bitmap_layer_create(GRect(114, 23, 30, 30));
  bitmap_layer_set_bitmap(s_bitmaplayer_2, s_res_arrow_up_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_2);
  
  // s_bitmaplayer_3
  s_bitmaplayer_3 = bitmap_layer_create(GRect(114, 99, 30, 30));
  bitmap_layer_set_bitmap(s_bitmaplayer_3, s_res_arrow_down_white);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_1);
  bitmap_layer_destroy(s_bitmaplayer_2);
  bitmap_layer_destroy(s_bitmaplayer_3);
  gbitmap_destroy(s_res_arrow_right_white);
  gbitmap_destroy(s_res_arrow_up_white);
  gbitmap_destroy(s_res_arrow_down_white);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_game_ddr(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_game_ddr(void) {
  window_stack_remove(s_window, true);
}
