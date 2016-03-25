#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;

static char get_suffix(int n)
{
  char ordinal[3];
  switch (n) {
    case 1: ordinal[0] = 's'; ordinal[1] = 't'; break;
    case 2: ordinal[0] = 'n'; ordinal[1] = 'd'; break;
    case 3: ordinal[0] = 'r'; ordinal[1] = 'd'; break;
    default: ordinal[0] = 't'; ordinal[1] = 'h';
  }
  return *ordinal;
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M%P", tick_time);

  // Write the current hours and minutes into a buffer
  static char s_date_buffer[13];
//   get_suffix(tick_time.tm_mday);
//   printf("BOOM %d", (*tick_time).tm_mday);
  
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d" + get_suffix((*tick_time).tm_mday) + " %b", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  printf("Width %d, Height %d", bounds.size.w, bounds.size.h);
  
  //Chalk, round - 180 x 180
  //Basalt, rectangular - 144 x 168
  
  // Create the TextLayer with specific bounds
//   s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 25));
  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(10, 0), bounds.size.w, 50));
  s_date_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(60, 50), bounds.size.w, 25));

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_40));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_20));
  
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayers & GFont
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_background_color(s_main_window, GColorBlack);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}