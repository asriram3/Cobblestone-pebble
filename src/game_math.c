#include "game_math.h"
#include "common.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_28;
static TextLayer *scr;
static TextLayer *quest;
static TextLayer *ans3;
static TextLayer *ans2;
static TextLayer *ans1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_gothic_28 = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  // scr
  scr = text_layer_create(GRect(40, 10, 144, 20));
  text_layer_set_background_color(scr, GColorClear);
  text_layer_set_text_color(scr, GColorWhite);
  text_layer_set_text(scr, "Score: ");
  layer_add_child(window_get_root_layer(s_window), (Layer *)scr);
  
  // quest
  quest = text_layer_create(GRect(16, 70, 100, 28));
  text_layer_set_background_color(quest, GColorClear);
  text_layer_set_text_color(quest, GColorWhite);
  text_layer_set_text(quest, "a + b = ?");
  text_layer_set_font(quest, s_res_gothic_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)quest);
  
  // ans3
  ans3 = text_layer_create(GRect(70, 10, 70, 40));
  text_layer_set_background_color(ans3, GColorClear);
  text_layer_set_text_color(ans3, GColorWhite);
  text_layer_set_text(ans3, "a<");
  text_layer_set_text_alignment(ans3, GTextAlignmentRight);
  text_layer_set_font(ans3, s_res_gothic_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)ans3);
  
  // ans2
  ans2 = text_layer_create(GRect(70, 70, 70, 40));
  text_layer_set_background_color(ans2, GColorClear);
  text_layer_set_text_color(ans2, GColorWhite);
  text_layer_set_text(ans2, "b<");
  text_layer_set_text_alignment(ans2, GTextAlignmentRight);
  text_layer_set_font(ans2, s_res_gothic_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)ans2);
  
  // ans1
  ans1 = text_layer_create(GRect(70, 120, 70, 40));
  text_layer_set_background_color(ans1, GColorClear);
  text_layer_set_text_color(ans1, GColorWhite);
  text_layer_set_text(ans1, "c<");
  text_layer_set_text_alignment(ans1, GTextAlignmentRight);
  text_layer_set_font(ans1, s_res_gothic_28);
  layer_add_child(window_get_root_layer(s_window), (Layer *)ans1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(scr);
  text_layer_destroy(quest);
  text_layer_destroy(ans3);
  text_layer_destroy(ans2);
  text_layer_destroy(ans1);
}
// END AUTO-GENERATED UI CODE


//ivars
static Layer *s_pad_layer;
static int score;
static int button_pressed;
static int a;
static int b;
static int c;
static int d;
static int e;
static int operation; //1:add, 2:sub, 3:mult
static char question[32];
static char ansa[32];
static char ansb[32];
static char ansc[32];
static char scor[32];
static char sign;
static int choice;
static int answer;
static AppTimer *timer;
static int elapsed;



static void get_answer(){
	switch(operation){
		case 1:
			sign = '+';
			answer = a + b;
			break;
		case 2:
			sign = '-';
			answer = a - b;
			break;
		case 3:
			sign = '*';
			answer = a * b;
			break;
		default:
			sign = 'a';
			answer = 23;
			break;
	}
	choice = rand()%4;
	int f = rand()%15;
	int g = rand()%15;
	if((f==answer)||(g==answer)){
			while(f==answer){
			f = rand()%15;
		}
		while(g==answer){
			g = rand()%15;
			while(!(g==f)){
				g = rand()%15;
			}
		}
	}
	
	
	if(choice==3){choice = 0;}
	
	if(choice==0){
		c = answer;
		d = f;
		e = g;
	}
	else if(choice==1){
		d = answer;
		c = f;
		e = g;
	}
	else if(choice==2){
		e = answer;
		d = f;
		c = g;
	}
	
	snprintf(question, sizeof(question), "%i %c %i = ?", a,sign, b);
	text_layer_set_text(quest, question);
	
	snprintf(ansa, sizeof(ansa), "%i<" , c);
	snprintf(ansb, sizeof(ansb), "%i<" , d);
	snprintf(ansc, sizeof(ansc), "%i<" , e);
	text_layer_set_text(ans1, ansa);
	text_layer_set_text(ans2, ansb);
	text_layer_set_text(ans3, ansc);
	
}

static void get_question(){
	srand(time(NULL));
	a = rand_in_range(1, 9);
	b = rand_in_range(1, 9);
	operation = rand_in_range(1, 3);
	get_answer();
}



static void pad_update_proc(Layer *this_layer, GContext *ctx){
	
	if(elapsed>155){
		out_of_time();
	}else{
		


		//Draw time bar
		graphics_context_set_fill_color(ctx, GColorWhite);
		graphics_fill_rect(ctx, GRect(0, elapsed , 7, 155-elapsed), 0, GCornerNone);

	//	APP_LOG(APP_LOG_LEVEL_DEBUG, "line 3");
		snprintf(scor, sizeof(scor), "Score: %i", score);
		text_layer_set_text(scr, scor);

		snprintf(question, sizeof(question), "%i %c %i = ?", a,sign, b);
		text_layer_set_text(quest, question);

		snprintf(ansa, sizeof(ansa), "%i<" , c);
		snprintf(ansb, sizeof(ansb), "%i<" , d);
		snprintf(ansc, sizeof(ansc), "%i<" , e);
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "line 4");
		text_layer_set_text(ans1, ansa);
		text_layer_set_text(ans2, ansb);
		text_layer_set_text(ans3, ansc);


	}
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "line 5");
	
}

void check_ans(){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "checking ans");
	if(button_pressed==(choice+1)){
				score += 1;
				if(score > 5){
					win();
				}
			}else{
				lose();
	}
	button_pressed = 0;
	get_question();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "checked ans");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
 	//text_layer_set_text(s_output_layer, "Up pressed!");
	button_pressed = 1;
	check_ans();
 	//layer_mark_dirty(s_pad_layer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_output_layer, "Select pressed!");
	button_pressed = 2;
	check_ans();
 	//layer_mark_dirty(s_pad_layer);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_output_layer, "Down pressed!");
	button_pressed = 3;
	check_ans();
 	//layer_mark_dirty(s_pad_layer);
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


static void timer_callback(){
	elapsed += 1;
	layer_mark_dirty(s_pad_layer);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "click");
	timer = app_timer_register(100, timer_callback, NULL);
}


static void init_stuff(){
	get_question();
	
	timer = app_timer_register(100, timer_callback, NULL);

	snprintf(scor, sizeof(scor), "Score: %i", score);
  text_layer_set_text(scr, scor);
	
	
}

static void handle_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
	s_pad_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
	
  layer_add_child(window_layer, s_pad_layer);
	
  // Set the update_proc
  layer_set_update_proc(s_pad_layer, pad_update_proc);
}

static void deinit_stuff(){
	layer_destroy(s_pad_layer);
	app_timer_cancel(timer);
}

static void handle_window_unload(Window* window) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "killing math");
  destroy_ui();
	deinit_stuff();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "math killed");
}

void show_game_math(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"starting math..");
  initialise_ui();
	score = 0;
	button_pressed = 0;
	elapsed = 0;
	init_stuff();
  window_set_window_handlers(s_window, (WindowHandlers) {
		.load = handle_window_load,
    .unload = handle_window_unload,
  });
	window_set_click_config_provider(s_window, click_config_provider);
	
  window_stack_push(s_window, true);
}

void hide_game_math(void) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"leaving math..");
  window_stack_remove(s_window, true);
	
}
