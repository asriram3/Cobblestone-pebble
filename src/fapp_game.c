#include <pebble.h>
#include "fapp_game.h"
#include "common.h"
#define ACCEL_STEP_MS 50

static Window *window;
static TextLayer *top_text;
static TextLayer *middle_text;
static BitmapLayer *image_layer;
Layer *window_layer;

GBitmap *arrow_up;
GBitmap *arrow_down;

int faps = 0;
double fpm = 0;
static AppTimer *timer;
AccelData accel;
int y_grav = 0;
bool pos = true;
int amount = 50;


void goToNextLevel(){
	//At this point the game is won. Send a win-ping to server and change game.
	APP_LOG(APP_LOG_LEVEL_DEBUG,"All I do is win.");
	faps = 0;
}

void setFapps(int i){
	amount = i;
}

void redraw_text(){
  //sets fap_value
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", (amount-faps));
  text_layer_set_text(middle_text, body_text);
	if(amount-faps<1){
		goToNextLevel();
	}
  //set fpm value  
	//fapp check here
}

void timer_callback(void *data) {
  accel_service_peek(&accel);
  y_grav = accel.y;
  if(pos){
    if(y_grav<0){
      pos=false;
			bitmap_layer_set_bitmap(image_layer, arrow_up);
      faps++;
    }
  }else{
    if(y_grav>0){
			bitmap_layer_set_bitmap(image_layer, arrow_down);
      pos=true;
      //faps++;
    }
  }
  redraw_text();
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

void fapp_game_init(){
		window = window_create();
		window_stack_push(window, true /* Animated */);
		window_layer = window_get_root_layer(window);
		GRect bounds = layer_get_bounds(window_layer);


		top_text = text_layer_create(bounds);
		middle_text = text_layer_create(bounds);
	
		// The bitmap layer holds the image for display
		image_layer = bitmap_layer_create(bounds);
		arrow_up = gbitmap_create_with_resource(RESOURCE_ID_ARROW_UP_WHITE);
		arrow_down = gbitmap_create_with_resource(RESOURCE_ID_ARROW_DOWN_WHITE);
		bitmap_layer_set_bitmap(image_layer, arrow_up);
		bitmap_layer_set_alignment(image_layer, GAlignCenter);
  
	
 
		text_layer_set_text_alignment(top_text, GTextAlignmentCenter);
		text_layer_set_text_alignment(middle_text, GTextAlignmentCenter);
		layer_add_child(window_layer, text_layer_get_layer(top_text));

		layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
		layer_add_child(window_layer, text_layer_get_layer(middle_text));

    //draw title
    GFont top_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_22));
    text_layer_set_font(top_text, top_font);
    text_layer_set_font(middle_text, top_font);
	
    text_layer_set_text(top_text, "SHAKE IT");
    //draw middle area
     GRect move_pos = (GRect) { .origin = { -15, 115 }, .size = { 180, 180 } };
    layer_set_frame(text_layer_get_layer(middle_text),move_pos);
    
	
    //draw number of faps
     static char body_text[50];
     snprintf(body_text, sizeof(body_text), "%u", amount-faps);
     text_layer_set_text(middle_text, body_text);    
   
		window_set_background_color(window, GColorBlack);
		
	
		text_layer_set_background_color(top_text, GColorBlack);
    text_layer_set_text_color(top_text, GColorWhite);
    text_layer_set_background_color(middle_text, GColorBlack);
    text_layer_set_text_color(middle_text, GColorWhite);
	
    accel_data_service_subscribe(0, NULL);
    timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

void fapp_game_deinit(){
	accel_data_service_unsubscribe();
  text_layer_destroy(top_text);
  text_layer_destroy(middle_text);
 	bitmap_layer_destroy(image_layer);
  window_destroy(window);
	app_timer_cancel(timer);
}

static void handle_window_unload(Window* window) {
	fapp_game_deinit();
}

void show_game_fapp(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"starting fapp..");
	fapp_game_init();
	
	APP_LOG(APP_LOG_LEVEL_DEBUG,"Setting window handles");
	window_set_window_handlers(window, (WindowHandlers) {
		.unload = handle_window_unload,
	});
	//window_set_click_config_provider(window, click_config_provider);
	APP_LOG(APP_LOG_LEVEL_DEBUG,"booty");
	window_stack_push(window, true);
	APP_LOG(APP_LOG_LEVEL_DEBUG,"am I still alive?");
}

void hide_game_fapp(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"LEAVING ddr..");
	window_stack_remove(window, true);
}
