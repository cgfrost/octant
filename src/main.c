#include <pebble.h>
#include "main_window.h"
#include "phone_window.h"

static void handle_tap(AccelAxisType axis, int32_t direction) {
  printf("BANG BANG TAP");
  phone_window_push();
  psleep(4000);
  window_stack_pop(true);
}

// Watchface setup

static void init() {
  main_window_init();
  phone_window_init();
  
  phone_window_push();
  
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
