#include <pebble.h>
#include "main_window.h"
#include "phone_window.h"

// TAPPING

static bool on_phone_screen = false;
static bool show_steps = false;

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

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  Tuple *show_steps_t = dict_find(iter, MESSAGE_KEY_SHOW_STEPS);
  if(show_steps_t) {
    show_steps = show_steps_t->value->int32 == 1;
  }

}

// LIFECYCLE

static void init() {
  main_window_init(show_steps);
  phone_window_init();  
//   accel_tap_service_subscribe(handle_tap);
    
//   on_phone_screen = true;
//   phone_window_push();
//   app_timer_register(3500, handle_timer, NULL);
  
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
