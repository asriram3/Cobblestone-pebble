
#include <pebble.h>
#include "game_bubs.h"
#include "common.h"
#include <stdio.h>
#include <math.h>

	
#define MATH_PI 3.141592653589793238462
#define NUM_DISCS 20
#define DISC_DENSITY 0.25
#define ACCEL_RATIO 0.05
#define ACCEL_STEP_MS 50

typedef struct Vec2d {
  double x;
  double y;
} Vec2d;

typedef struct Disc {
#ifdef PBL_COLOR
  GColor color;
#endif
  Vec2d pos;
  Vec2d vel;
  double mass;
  double radius;
} Disc;



static Window *s_main_window;
static Layer *s_disc_layer;

static Disc disc_array[NUM_DISCS];
static GRect window_frame;
static int level = 10;
static int portal_x;// = rand() % 100+20;
static int portal_y;// = rand() % 100+20;
static AppTimer *timer_3;

double disc_calc_mass(Disc *disc) {
  return MATH_PI * disc->radius * disc->radius * DISC_DENSITY;
}

void disc_init(Disc *disc) {
  double next_radius = 3;

  GRect frame = window_frame;
  disc->pos.x = frame.size.w/2;
  disc->pos.y = frame.size.h/2;
  disc->vel.x = 0;
  disc->vel.y = 0;
  disc->radius = next_radius;
  disc->mass = disc_calc_mass(disc);
#ifdef PBL_COLOR
  disc->color = GColorFromRGB(rand() % 255, rand() % 255, rand() % 255);
#endif
  next_radius += 0.5;
}

void disc_apply_force(Disc *disc, Vec2d force) {
  disc->vel.x += force.x / disc->mass;
  disc->vel.y += force.y / disc->mass;
}

void disc_apply_accel(Disc *disc, AccelData accel) {
  disc_apply_force(disc, (Vec2d) {
    .x = accel.x * ACCEL_RATIO,
    .y = -accel.y * ACCEL_RATIO
  });
}

void disc_update(Disc *disc) {
  double e = 0.5;

  if ((disc->pos.x - disc->radius < 0 && disc->vel.x < 0)
    || (disc->pos.x + disc->radius > window_frame.size.w && disc->vel.x > 0)) {
    disc->vel.x = -disc->vel.x * e;
  }

  if ((disc->pos.y - disc->radius < 0 && disc->vel.y < 0)
    || (disc->pos.y + disc->radius > window_frame.size.h && disc->vel.y > 0)) {
    disc->vel.y = -disc->vel.y * e;
  }

  disc->pos.x += disc->vel.x;
  disc->pos.y += disc->vel.y;
}

void disc_draw(GContext *ctx, Disc *disc) {
#ifdef PBL_COLOR
  graphics_context_set_fill_color(ctx, disc->color);
#else
  graphics_context_set_fill_color(ctx, GColorWhite);
#endif
	graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(disc->pos.x, disc->pos.y), disc->radius);
	graphics_draw_circle(ctx, GPoint(portal_x, portal_y), disc->radius);
	
	double dx = (portal_x-disc->pos.x);
	double dy = (portal_y-disc->pos.y);
	double dist = dx*dx + dy*dy;
	if(dist<225){
//		APP_LOG(APP_LOG_LEVEL_DEBUG,"All I do is win.");
		win();
	}
	
}

void disc_layer_update_callback(Layer *me, GContext *ctx) {
 
    disc_draw(ctx, &disc_array[level]);
  
}

void timer_callback_2(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);

  for (int i = 0; i < NUM_DISCS; i++) {
    Disc *disc = &disc_array[i];
    disc_apply_accel(disc, accel);
    disc_update(disc);
  }

  layer_mark_dirty(s_disc_layer);

  timer_3 = app_timer_register(ACCEL_STEP_MS, timer_callback_2, NULL);
}

void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect frame = window_frame = layer_get_frame(window_layer);

  s_disc_layer = layer_create(frame);
  layer_set_update_proc(s_disc_layer, disc_layer_update_callback);
  layer_add_child(window_layer, s_disc_layer);

  for (int i = 0; i < NUM_DISCS; i++) {
    disc_init(&disc_array[i]);
  }
}

void main_window_unload(Window *window) {
  layer_destroy(s_disc_layer);
}


void bubble_init(void) {
	portal_x = rand() % 100+20;
  portal_y = rand() % 100+20;
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  accel_data_service_subscribe(0, NULL);

  timer_3 = app_timer_register(ACCEL_STEP_MS, timer_callback_2, NULL);
	
	// add time remaining layer
	layer_add_child(window_get_root_layer(s_main_window), g_time_layer);
}

void bubble_deinit(void) {
	app_timer_cancel(timer_3);
  accel_data_service_unsubscribe();
	window_stack_remove(s_main_window, true);
  window_destroy(s_main_window);
}

void show_game_bubs(void){
	bubble_init();
}
void hide_game_bubs(void){
	bubble_deinit();
}