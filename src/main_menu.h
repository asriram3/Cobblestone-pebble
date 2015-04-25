#pragma once
#include <pebble.h>


void go_black();
void go_white();
void menu_change_info(char *info);
void menu_handle_feedback(char *feed);
void middle_click_handler(ClickRecognizerRef recognizer, void *context);
void config_provider(void *context); 
void init_menu();
void menu_deinit();
