#include <pebble.h>
#include "main_window.h"

static Window *s_main_window;
static TextLayer *s_temp_layer, *s_time_layer, *s_ampm_layer, *s_date_layer, *s_battery_layer, *s_activity_layer;
static BitmapLayer *s_battery_image_layer, *s_bluetooth_image_layer;
static GFont s_big_font, s_medium_font, s_smedium_font, s_small_font;
static GBitmap *s_bluetooth_bitmap, *s_bluetooth_connected_bitmap, *s_battery_charging_bitmap, *s_battery_good_bitmap, *s_battery_low_bitmap, *s_battery_warning_bitmap;

static bool show_steps = false;
const HealthMetric stepCountMetric = HealthMetricStepCount;

// Handlers

static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[] = "100% charged";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "----");
    bitmap_layer_set_bitmap(s_battery_image_layer, s_battery_charging_bitmap);
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
    if(charge_state.charge_percent <= 10) {
      bitmap_layer_set_bitmap(s_battery_image_layer, s_battery_warning_bitmap);
    } else {
      if(charge_state.charge_percent <= 20) {
        bitmap_layer_set_bitmap(s_battery_image_layer, s_battery_low_bitmap);
      } else {
        bitmap_layer_set_bitmap(s_battery_image_layer, s_battery_good_bitmap);
      }
    }
  }
  text_layer_set_text(s_battery_layer, battery_text);
}

static void handle_bluetooth(bool connected) {
  if(connected) {
      bitmap_layer_set_bitmap(s_bluetooth_image_layer, s_bluetooth_connected_bitmap);
  } else {
      bitmap_layer_set_bitmap(s_bluetooth_image_layer, s_bluetooth_bitmap);
  }
}

static void update_steps() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "updating display steps boolean: %d", (int) show_steps);
  
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(stepCountMetric, start, end);
  bool any_data_available = mask & HealthServiceAccessibilityMaskAvailable;
  
  static char steps_text[] = "-----";
  if(show_steps){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SHOW STEPS IS TRUE");
  }
  if(any_data_available){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "ANY DATA IS TRUE");
  }
  if(show_steps == 1 && any_data_available) { // WTF
    int steps = health_service_sum_today(stepCountMetric);
    snprintf(steps_text, sizeof steps_text, "%d", steps);
  }
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "updating display steps to text: %s", steps_text);
  text_layer_set_text(s_activity_layer, steps_text);
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char s_time_buffer[6];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  static char s_ampm_buffer[3];
  if(! clock_is_24h_style()) {
    strftime(s_ampm_buffer, sizeof(s_ampm_buffer), "%P", tick_time);
  }
  
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
  text_layer_set_text(s_ampm_layer, s_ampm_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
  
  update_steps();
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
  
  // Chalk, Pebble Time Round                - 180 x 180
  // Basalt, Pebble Time & Pebble Time Steel - 144 x 168
  // Diorite, Pebble 2                       - 144 x 168
  // Emery, Pebble Time 2                    - 200 x 228
  
  //Fonts
  s_big_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_45));
  s_medium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_24));
  s_smedium_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_20));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_REGULAR_15));
  
  //Bitmaps
  s_bluetooth_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_DISCONNECTED);
  s_bluetooth_connected_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BLUETOOTH_CONNECTED); 
  s_battery_charging_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_CHARGING);
  s_battery_good_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_GOOD);
  s_battery_low_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_LOW);
  s_battery_warning_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BATTERY_WARNING);
  
  //Create Layers
  s_time_layer = build_text_layer(s_big_font, GRect(0, PBL_IF_ROUND_ELSE(13, 6), bounds.size.w, 50));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  s_ampm_layer = build_text_layer(s_small_font, GRect(PBL_IF_ROUND_ELSE(137, 118), PBL_IF_ROUND_ELSE(43, 36), 27, 20));
  layer_add_child(window_layer, text_layer_get_layer(s_ampm_layer));
  
  s_date_layer = build_text_layer(s_medium_font, GRect(0, PBL_IF_ROUND_ELSE(74, 68), bounds.size.w, 30));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  s_activity_layer = build_text_layer(s_medium_font, GRect(0, PBL_IF_ROUND_ELSE(103, 100), bounds.size.w, 30));
  layer_add_child(window_layer, text_layer_get_layer(s_activity_layer));
  
  s_battery_image_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(16, 0), 138, 40, 20));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_battery_image_layer));
  
  s_battery_layer = build_text_layer(s_smedium_font, GRect(PBL_IF_ROUND_ELSE(58, 42), 135, 50, 20));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  
  s_bluetooth_image_layer = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(123, 104), 138, 20, 20));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bluetooth_image_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_activity_layer);
  text_layer_destroy(s_battery_layer);
  fonts_unload_custom_font(s_big_font);
  fonts_unload_custom_font(s_medium_font);
  fonts_unload_custom_font(s_small_font);
  gbitmap_destroy(s_bluetooth_bitmap);
  gbitmap_destroy(s_bluetooth_connected_bitmap);
  gbitmap_destroy(s_battery_charging_bitmap);
  gbitmap_destroy(s_battery_good_bitmap);
  gbitmap_destroy(s_battery_low_bitmap);
  gbitmap_destroy(s_battery_warning_bitmap);
  bitmap_layer_destroy(s_battery_image_layer);
  bitmap_layer_destroy(s_bluetooth_image_layer);
}

void set_display_show_steps(bool show_steps_config) {
  show_steps = show_steps_config;
  update_steps();
}

void main_window_init(bool show_steps_config) {
  show_steps = show_steps_config;
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

