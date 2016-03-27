#include <pebble.h>
  
static Window *s_main_window;
static Window *s_phone_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_connection_layer;
static TextLayer *s_phone_layer;
static GFont s_time_font, s_date_font, s_phone_font;

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
  static char template[] = "%a %d-- %b";
  
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

static void handle_tap(AccelAxisType axis, int32_t direction) {
  printf("BANG BANG TAP");
  window_stack_push(s_phone_window, true);
  psleep(4000);
  window_stack_pop(true);
}

// Setup Phone Window

static void phone_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_phone_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_40));

  s_phone_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(10, 0), bounds.size.w, 50));
  text_layer_set_background_color(s_phone_layer, GColorClear);
  text_layer_set_text_color(s_phone_layer, GColorWhite);
  text_layer_set_font(s_phone_layer, s_phone_font);
  text_layer_set_text_alignment(s_phone_layer, GTextAlignmentCenter);
  text_layer_set_text(s_phone_layer, "BOOM");
  layer_add_child(window_layer, text_layer_get_layer(s_phone_layer)); 
}

static void phone_window_unload(Window *window) {
  text_layer_destroy(s_phone_layer);
  fonts_unload_custom_font(s_phone_font);
}

// Setup Main Window

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Chalk, round - 180 x 180
  //Basalt, rectangular - 144 x 168
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_40));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_22));
  
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(10, 0), bounds.size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(60, 50), bounds.size.w, 25));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  s_connection_layer = text_layer_create(GRect(0, 90, bounds.size.w, 34));
  text_layer_set_text_color(s_connection_layer, GColorWhite);
  text_layer_set_background_color(s_connection_layer, GColorClear);
  text_layer_set_font(s_connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_connection_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_connection_layer));
  
  s_battery_layer = text_layer_create(GRect(0, 120, bounds.size.w, 34));
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_connection_layer);
  text_layer_destroy(s_battery_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
}

// Watchface setup

static void init() {
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
  accel_tap_service_subscribe(handle_tap);
    
  // Setup phone window
  s_phone_window = window_create();

  window_set_window_handlers(s_phone_window, (WindowHandlers) {
    .load = phone_window_load,
    .unload = phone_window_unload
  });

  window_set_background_color(s_phone_window, GColorBlack); 
  
  window_stack_push(s_phone_window, true);
  psleep(4000);
  window_stack_pop(true);
}

static void deinit() {
  window_destroy(s_main_window);
  window_destroy(s_phone_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
