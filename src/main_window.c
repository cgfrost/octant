#include <pebble.h>
#include "main_window.h"

static Window *s_main_window;
static TextLayer *s_temp_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_connection_layer;
static GFont s_big_font, s_medium_font, s_small_font;

// Handlers

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "100% charged";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "charging");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%% charged", charge_state.charge_percent);
  }
  text_layer_set_text(s_battery_layer, battery_text);
}

static void handle_bluetooth(bool connected) {
  text_layer_set_text(s_connection_layer, connected ? "connected" : "disconnected");
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M%P", tick_time);

  static char s_date_buffer[13];
  static char template[] = "%a %e-- %b";
  
  switch ((*tick_time).tm_mday) {
    case 1: template[5] = 's'; template[6] = 't'; break;
    case 21: template[5] = 's'; template[6] = 't'; break;
    case 31: template[5] = 's'; template[6] = 't'; break;
    case 2: template[5] = 'n'; template[6] = 'd'; break;
    case 22: template[5] = 'n'; template[6] = 'd'; break;
    case 3: template[5] = 'r'; template[6] = 'd'; break;
    case 23: template[5] = 'r'; template[6] = 'd'; break;
    default: template[5] = 't'; template[6] = 'h';
  }
        
  strftime(s_date_buffer, sizeof(s_date_buffer), template, tick_time);
  
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

// Helper

static TextLayer* build_text_layer(GFont font, GRect frame) {
  s_temp_layer = text_layer_create(frame);
  text_layer_set_background_color(s_temp_layer, GColorClear);
  text_layer_set_text_color(s_temp_layer, GColorWhite);
  text_layer_set_font(s_temp_layer, font);
  text_layer_set_text_alignment(s_temp_layer, GTextAlignmentCenter);
  return s_temp_layer;
}

// Setup Main Window

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Chalk, round - 180 x 180
  //Basalt, rectangular - 144 x 168
  
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_45));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_25));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_15));
  
  s_time_layer = build_text_layer(s_big_font, GRect(0, PBL_IF_ROUND_ELSE(16, 10), bounds.size.w, 50));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  s_date_layer = build_text_layer(s_medium_font, GRect(0, PBL_IF_ROUND_ELSE(75, 69), bounds.size.w, 30));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  s_connection_layer = build_text_layer(s_small_font, GRect(0, 105, bounds.size.w, 20));
  layer_add_child(window_layer, text_layer_get_layer(s_connection_layer));
  
  s_battery_layer = build_text_layer(s_small_font, GRect(0, 125, bounds.size.w, 20));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));  
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_temp_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_connection_layer);
  text_layer_destroy(s_battery_layer);
  fonts_unload_custom_font(s_big_font);
  fonts_unload_custom_font(s_medium_font);
  fonts_unload_custom_font(s_small_font);
}

void main_window_init() {
    s_main_window = window_create();

    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
    });
  
  window_set_background_color(s_main_window, GColorBlack);
  window_stack_push(s_main_window, true);
  
  // Setup initial values
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  
  handle_tick(current_time, MINUTE_UNIT);
  handle_battery(battery_state_service_peek());
  handle_bluetooth(connection_service_peek_pebble_app_connection());
  
  // Register Services  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  battery_state_service_subscribe(handle_battery);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_bluetooth
  });
}

void main_window_deinit() {
  window_destroy(s_main_window);
}

