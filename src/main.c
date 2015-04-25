#include <pebble.h>
#include <string.h>
#include "appmessage.h"
#include "common.h"
#include "game_shape_memory.h"
#include "game_ddr.h"
#include "main_menu.h"


#define ACCEL_STEP_MS	50

bool menu				= true;
int faps				= 0;
double fpm				= 0;
AccelData accel;
int y_grav				= 0;
bool pos				= true;
double time_elapsed		= 0;
char lobby_status[64]	= "test";


// Games
typedef void (* GameShow)(void);
typedef void (* GameHide)(void);
static const int numGames = 2;
static void startPlaying();
static void changeGame();
static GameHide s_gameHideFunc;
static GameShow gameShowForIndex(const int index);
static GameHide gameHideForIndex(const int index);

static GameShow gameShowForIndex(const int index) {
	switch (index) {
	case 0:
		return show_game_ddr;
	case 1:
		return show_game_shape_memory;
	default:
		return NULL;
	}
}
static GameHide gameHideForIndex(const int index) {
	switch (index) {
	case 0:
		return hide_game_ddr;
	case 1:
		return hide_game_shape_memory;
	default:
		return NULL;
	}
}

static void startPlaying() {
	changeGame();
}

static void changeGame() {
	// close the current minigame
	if (s_gameHideFunc) {
		s_gameHideFunc();
	}
	// select the next minigame
	const int randIndex = rand() % numGames;
	GameShow gameShow = gameShowForIndex(randIndex);
	// save the callback to close the new minigame
	s_gameHideFunc = gameHideForIndex(randIndex);
	// open the next minigame after a delay
	if (gameShow) {
		app_timer_register(1500, (AppTimerCallback)gameShow, NULL);
	}
}

	// complete a minigame successfully
void win() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You win!");
	appmesg_send_win();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s()", __func__);
	changeGame();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s()", __func__);
}

	// lose a minigame
void lose() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You lose!");
	// no need to send a loss
	changeGame();
}

	// run out of time in a minigame
void out_of_time() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You died!");
	appmesg_send_death();
	// close the current minigame
	s_gameHideFunc();
	s_gameHideFunc = NULL;
}


// AppMessage
static void inbox_received_callback(DictionaryIterator *iterator, void *context) { 
	// Get the first pair 
	Tuple *tuple = dict_read_first(iterator);
	
	// process all pairs present 
	while (tuple) { 
		// long-lived buffer 
		static char prevGameStatus[64]; 
		static char s_buffer1[64]; 
		static char s_buffer2[64];

		// process this pair's key 
		switch (tuple->key) { 
		case APPMESG_PLAYERS:
			snprintf(s_buffer2, sizeof(s_buffer2), "/%s", tuple->value->cstring); 
			menu_handle_feedback(tuple->value->cstring);
			break; 
		case APPMESG_READY:
			snprintf(s_buffer1, sizeof(s_buffer1), "Players Ready: %s", tuple->value->cstring); 
			break; 
		case APPMESG_GAME_STATUS:
			; // can't declare a var immediately after a label ("case gameStatus:")
			char newGameStatus[64];
			snprintf(newGameStatus, sizeof(prevGameStatus), "%s", tuple->value->cstring);
			// check for a new game status
			if (strncmp(newGameStatus, prevGameStatus, sizeof(newGameStatus))) {
				// check if the new status is playing
				if (!strncmp(newGameStatus, "Playing", sizeof(newGameStatus))) {
					// finish processing this message before we start playing the next game
					app_timer_register(5, startPlaying, NULL);
				}
			}
			// copy the new status into the previous status buffer
			strncpy(prevGameStatus, newGameStatus, sizeof(newGameStatus));
			
			break; 
		}
		snprintf(lobby_status, sizeof(lobby_status), "%s%s", s_buffer1, s_buffer2);
		menu_change_info(lobby_status);
		 
		// get the next pair, if any 
		tuple = dict_read_next(iterator); 
	} 
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
//	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
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
}

static void deinit() {
	fonts_unload_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_35));
	fonts_unload_custom_font(resource_get_handle(RESOURCE_ID_FONT_FAP_40));
	fonts_unload_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_20));  
	fonts_unload_custom_font(resource_get_handle(RESOURCE_ID_FONT_DEFAULT_18));  
	
	accel_data_service_unsubscribe();
	
	game_deinit();
	
	menu_deinit();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
