#include <pebble.h>
#include "main_window.h"
#include "phone_window.h"

enum WeatherKey {
  VIBRATE_ON_DISCONNECT = 0x0,  // TUPLE_INT
  DEVICE_WIFI_LEVEL = 0x1,      // TUPLE_CSTRING
  DEVICE_WIFI_NAME = 0x2,       // TUPLE_CSTRING
  DEVICE_CELL_LEVEL = 0x3,
  DEVICE_CELL_NAME = 0x4,
  DEVICE_BATTERY_NAME = 0x5,
};

// Largest expected inbox and outbox message sizes
const uint32_t inbox_size = 54;
const uint32_t outbox_size = 256;

// Open AppMessage
app_message_open(inbox_size, outbox_size);

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  // A new message has been successfully received

}




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

static void init() {
  main_window_init();
  phone_window_init();  
  
  app_message_register_inbox_received(inbox_received_callback);
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
