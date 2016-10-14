#pragma once

// static void handle_battery(BatteryChargeState charge_state);
// static void handle_bluetooth(bool connected);
// static void update_steps();
// static void handle_tick(struct tm *tick_time, TimeUnits units_changed);
// static TextLayer* build_text_layer(GFont font, GRect frame);
// static void main_window_load(Window *window);
// static void main_window_unload(Window *window);

void set_display_show_steps(bool show_steps_config);
void main_window_init();
void main_window_deinit();
