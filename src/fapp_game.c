#include <pebble.h>
#include "fapp_game.h"
#include "common.h"
#define ACCEL_STEP_MS 50

static Window *window_2;
static TextLayer *top_text_2;
static TextLayer *middle_text_2;
static BitmapLayer *image_layer_2;
Layer *window_layer_2;

GBitmap *arrow_up;
GBitmap *arrow_down;

int faps_2 = 0;
double fpm_2 = 0;
static AppTimer *timer_2;
AccelData accel_2;
int y_grav_2 = 0;
bool pos_2 = true;
int amount_2 = 50;



void goToNextLevel(){
	//At this point the game is won. Send a win-ping to server and change game.
	APP_LOG(APP_LOG_LEVEL_DEBUG,"All I do is win.");
	win();
	faps_2 = 0;
}

void setFapps(int i){
	amount_2 = i;
}

void redraw_text(){
  //sets fap_value
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", (amount_2-faps_2));
  text_layer_set_text(middle_text_2, body_text);
	if(amount_2-faps_2<1){
		goToNextLevel();
	}
  //set fpm value  
	//fapp check here
}

void timer_callback(void *data) {
  accel_service_peek(&accel_2);
  y_grav_2 = accel_2.y;
  if(pos_2){
    if(y_grav_2<0){
      pos_2=false;
			bitmap_layer_set_bitmap(image_layer_2, arrow_up);
      faps_2++;
    }
  }else{
    if(y_grav_2>0){
			bitmap_layer_set_bitmap(image_layer_2, arrow_down);
      pos_2=true;
      //faps++;
    }
  }
  redraw_text();
  timer_2 = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

void fapp_game_init(){
	
		window_2 = window_create();
		window_stack_push(window_2, true );
		window_layer_2 = window_get_root_layer(window_2);
		GRect bounds = layer_get_bounds(window_layer_2);


		top_text_2 = text_layer_create(bounds);
		middle_text_2 = text_layer_create(bounds);
	
		// The bitmap layer holds the image for display
		image_layer_2 = bitmap_layer_create(bounds);
		arrow_up = gbitmap_create_with_resource(RESOURCE_ID_ARROW_UP_WHITE);
		arrow_down = gbitmap_create_with_resource(RESOURCE_ID_ARROW_DOWN_WHITE);
		bitmap_layer_set_bitmap(image_layer_2, arrow_up);
		bitmap_layer_set_alignment(image_layer_2, GAlignCenter);
  
	
 
		text_layer_set_text_alignment(top_text_2, GTextAlignmentCenter);
		text_layer_set_text_alignment(middle_text_2, GTextAlignmentCenter);
		layer_add_child(window_layer_2, text_layer_get_layer(top_text_2));

		layer_add_child(window_layer_2, bitmap_layer_get_layer(image_layer_2));
		layer_add_child(window_layer_2, text_layer_get_layer(middle_text_2));

    //draw title
    GFont top_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_22));
    text_layer_set_font(top_text_2, top_font);
    text_layer_set_font(middle_text_2, top_font);
	
    text_layer_set_text(top_text_2, "SHAKE IT");
    //draw middle area
     GRect move_pos = (GRect) { .origin = { -15, 115 }, .size = { 180, 180 } };
    layer_set_frame(text_layer_get_layer(middle_text_2),move_pos);
    
	
    //draw number of faps
     static char body_text[50];
     snprintf(body_text, sizeof(body_text), "%u", amount_2-faps_2);
     text_layer_set_text(middle_text_2, body_text);    
   
		window_set_background_color(window_2, GColorBlack);
		
	
		text_layer_set_background_color(top_text_2, GColorBlack);
    text_layer_set_text_color(top_text_2, GColorWhite);
    text_layer_set_background_color(middle_text_2, GColorBlack);
    text_layer_set_text_color(middle_text_2, GColorWhite);
	
    accel_data_service_subscribe(0, NULL);
    timer_2 = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
		
}

void fapp_game_deinit(){
	
	accel_data_service_unsubscribe();
  text_layer_destroy(top_text_2);
  text_layer_destroy(middle_text_2);
 	bitmap_layer_destroy(image_layer_2);
  window_destroy(window_2);
	app_timer_cancel(timer_2);
	
}

static void handle_window_unload(Window* window) {
	fapp_game_deinit();
}

void show_game_fapp(void) {
	
	APP_LOG(APP_LOG_LEVEL_DEBUG,"starting fapp..");
	fapp_game_init();
	
	APP_LOG(APP_LOG_LEVEL_DEBUG,"Setting window handles");
	window_set_window_handlers(window_2, (WindowHandlers) {
		.unload = handle_window_unload,
	});
	//window_set_click_config_provider(window, click_config_provider);
	APP_LOG(APP_LOG_LEVEL_DEBUG,"booty");
	window_stack_push(window_2, true);
	APP_LOG(APP_LOG_LEVEL_DEBUG,"am I still alive?");
	
}

void hide_game_fapp(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"LEAVING ddr..");
	window_stack_remove(window_2, true);
}
