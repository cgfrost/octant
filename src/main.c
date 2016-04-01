#include <pebble.h>
#include "main_window.h"
#include "phone_window.h"

static bool on_phone_screen = false;

static void handle_timer(void* data) {
  window_stack_pop(true);
  on_phone_screen = false;
}

static void handle_tap(AccelAxisType axis, int32_t direction) {
  if(!on_phone_screen){
    on_phone_screen = true;
    phone_window_push();
    app_timer_register(4000, handle_timer, NULL);
  }
}

static void init() {
  main_window_init();
  phone_window_init();  
  accel_tap_service_subscribe(handle_tap);
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
