#include <pebble.h>
#include "phone_window.h"

static Window *s_phone_window;
static TextLayer *s_phone_layer;
static GFont s_phone_font;

//Logo

static BitmapLayer *s_logo_layer;
static GBitmap *s_logo_bitmap;


// Setup Phone Window - 

  //Chalk, round - 180 x 180
  //Basalt, rectangular - 144 x 168

static void phone_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_phone_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOURCE_SANS_PRO_LIGHT_45));

  // Create GBitmap
  s_logo_layer = bitmap_layer_create(GRect(0, PBL_IF_ROUND_ELSE(10, 5), bounds.size.w, 50));
  s_logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_M_JOLLA);
  bitmap_layer_set_bitmap(s_logo_layer, s_logo_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_logo_layer));
  
  
  s_phone_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(70, 65), bounds.size.w, 50));
  text_layer_set_background_color(s_phone_layer, GColorClear);
  text_layer_set_text_color(s_phone_layer, GColorWhite);
  text_layer_set_font(s_phone_layer, s_phone_font);
  text_layer_set_text_alignment(s_phone_layer, GTextAlignmentCenter);
  text_layer_set_text(s_phone_layer, "@$#£!");
  layer_add_child(window_layer, text_layer_get_layer(s_phone_layer));
  
}

static void phone_window_unload(Window *window) {
  fonts_unload_custom_font(s_phone_font);
  text_layer_destroy(s_phone_layer);
  gbitmap_destroy(s_logo_bitmap);
  bitmap_layer_destroy(s_logo_layer);
}

void phone_window_init() {
  if(!s_phone_window) {
    s_phone_window = window_create();
    
    window_set_window_handlers(s_phone_window, (WindowHandlers) {
      .load = phone_window_load,
      .unload = phone_window_unload,
    });
  }
  window_set_background_color(s_phone_window, GColorBlack); 
}

void phone_window_push() {
  window_stack_push(s_phone_window, true);
}

void phone_window_deinit() {
  window_destroy(s_phone_window);
}
