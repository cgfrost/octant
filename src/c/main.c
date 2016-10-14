#include <pebble.h>
#include "main.h"
#include "main_window.h"
#include "phone_window.h"

// TAPPING

static bool on_phone_screen = false;

static void handle_timer(void* data) {
  window_stack_pop(true);
  on_phone_screen = false;
}

static void handle_tap(AccelAxisType axis, int32_t direction) {
  if(!on_phone_screen){
    on_phone_screen = true;
    phone_window_push();
    app_timer_register(3500, handle_timer, NULL);
  }
}

// SETTINGS (see main.h)
ClaySettings settings;

static void prv_load_settings() {
  // Load the default settings
  settings.show_tap_screen = true;
  settings.show_steps  = true;
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  Tuple *show_steps_t = dict_find(iter, MESSAGE_KEY_SHOW_STEPS);
  if(show_steps_t) {
    settings.show_steps = show_steps_t->value->int32 == 1;
    set_display_show_steps(settings.show_steps);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "steps set to: %d", (int) settings.show_steps);
  }
  
  Tuple *show_tap_screen_t = dict_find(iter, MESSAGE_KEY_SHOW_TAP_SCREEN);
  if(show_tap_screen_t) {
    settings.show_tap_screen = show_tap_screen_t->value->int32 == 1;
    if(settings.show_tap_screen) {
      accel_tap_service_subscribe(handle_tap);
    } else{
      accel_tap_service_unsubscribe();
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "tap screen set to: %d", (int) settings.show_tap_screen);
  }
  
  // Save the new settings to persistent storage
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// LIFECYCLE

static void init() {
  prv_load_settings();
  main_window_init();
  phone_window_init();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Launch steps: %d", (int) settings.show_steps);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Launch tap screen: %d", (int) settings.show_tap_screen);
  set_display_show_steps(settings.show_steps);
  if(settings.show_tap_screen) {
    accel_tap_service_subscribe(handle_tap);
  } 
    
  on_phone_screen = true;
  phone_window_push();
  app_timer_register(3500, handle_timer, NULL);
  
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}


static void deinit() {
  main_window_deinit();
  phone_window_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
