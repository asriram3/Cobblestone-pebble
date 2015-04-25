#include <pebble.h>
#include <string.h>
#include "common.h"
#include "game_shape_memory.h"
#include "game_ddr.h"
#include "main_menu.h"


#define players			0
#define ready			1
#define gameStatus		2
#define flag			3


#define ACCEL_STEP_MS	50


bool menu				= true;
int faps				= 0;
double fpm				= 0;
AccelData accel;
int y_grav				= 0;
bool pos				= true;
double time_elapsed		= 0;
char lobby_status[64]	= "test";


// AppMessage
static void inbox_received_callback(DictionaryIterator *iterator, void *context) { 
	// Get the first pair 
	Tuple *t = dict_read_first(iterator);
	
	// Process all pairs present 
	while (t != NULL ) { 
		// Long lived buffer 
		static char s_buffer[64]; 
		static char s_buffer1[64]; 
		static char s_buffer2[64];

		// Process this pair's key 
			switch (t->key) { 
			case players:
				snprintf(s_buffer2, sizeof(s_buffer), "/%s", t->value->cstring); 
				menu_handle_feedback(t->value->cstring);
				break; 
			case ready:
				snprintf(s_buffer1, sizeof(s_buffer), "Players Ready:%s", t->value->cstring); 
				break; 
			case gameStatus:
				break; 
		}
		snprintf(lobby_status, sizeof(lobby_status), "%s%s", s_buffer1, s_buffer2);
		menu_change_info(lobby_status);
		 
		// Get next pair, if any 
		t = dict_read_next(iterator); 
	} 
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void init() {
	srand(time(NULL)); // seed pRNG
	
	// Register callbacks 
	app_message_register_inbox_received(inbox_received_callback); 
	app_message_register_inbox_dropped(inbox_dropped_callback); 
	app_message_register_outbox_failed(outbox_failed_callback); 
	app_message_register_outbox_sent(outbox_sent_callback); 
	// Open AppMessage 
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	init_menu();
	
	game_init();
	
//	show_game_shape_memory();
//	show_game_ddr();
}

static void deinit() {
	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_35));
	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_40));
	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_20));  
	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_18));  
	
	accel_data_service_unsubscribe();
	
	game_deinit();
	
	menu_deinit();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
