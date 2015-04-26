#pragma once
#include "pebble.h"
void fapp_game_init(); //Initializes game
void fapp_game_deinit(); //Deinitialize game
void timer_callback(void *data); //Runs every 50ms
void redraw_text(); // Redraws text of window
void goToNextLevel(); //Triggered when level is beaten
void setFapps(int i); //Set the number of fapps to stroke
void show_game_fapp(void);
void hide_game_fapp(void);