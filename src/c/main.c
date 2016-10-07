#include <pebble.h>
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

// LIFECYCLE

static void init() {
  main_window_init();
  phone_window_init();  
//   accel_tap_service_subscribe(handle_tap);
    
//   on_phone_screen = true;
//   phone_window_push();
//   app_timer_register(3500, handle_timer, NULL);
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
