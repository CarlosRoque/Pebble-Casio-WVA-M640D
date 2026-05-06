#include <pebble.h>
#include "config.h"

static Window *s_window;
static BitmapLayer *s_bg_layer;
static GBitmap *s_bg_bitmap;
static RotBitmapLayer *s_hour_hand_layer;
static GBitmap *s_hour_hand_bitmap;
static RotBitmapLayer *s_minute_hand_layer;
static GBitmap *s_minute_hand_bitmap;
static RotBitmapLayer *s_second_hand_layer;
static GBitmap *s_second_hand_bitmap;
static TextLayer *s_day_layer;
static GFont s_large_font;
static GFont s_small_font;
static GFont s_system_font;
static TextLayer *s_day_num_layer;
static Layer *s_dst_layer;
static Layer *s_bt_layer;
static Layer *s_hands_pivot_layer;
static TextLayer *s_weather_icon_layer;
static TextLayer *s_weather_temp_layer;
static GFont s_weather_font;
static char s_weather_icon_buf[2] = "I";
static char s_weather_temp_buf[10] = "---";
static bool s_weather_loaded = false;
static int16_t s_last_temp_c = 0;
static bool s_units_fahrenheit = false;
static bool s_vib_bt = true;
static BitmapLayer *s_batt_icon_layer;
static GBitmap *s_batt_all;
static GBitmap *s_batt_frame;
static TextLayer *s_batt_text_layer;
static char s_batt_text_buf[7] = "---";
static bool s_batt_display_pct = false;
#define SECONDS_ALWAYS    0
#define SECONDS_NEVER     1
#define SECONDS_FLICK     2
#define BACKLIGHT_DURATION_MS 5000

static int s_seconds_mode = SECONDS_ALWAYS;
static bool s_seconds_visible = true;
static AppTimer *s_seconds_timer = NULL;

// 12-hour cycle = 720 minutes = 360 degrees, so 1 degree = 2 minutes.
static const char *day_string(int wday) {
  static const char *days[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
  return days[wday];
}

static const char *day_num_string(int mday) {
  static char buffer[3];
  snprintf(buffer, sizeof(buffer), "%02d", mday);
  return buffer;
}

static int32_t hours_angle(struct tm *t) {
  int total_minutes = (t->tm_hour % 12) * 60 + t->tm_min;
  return DEG_TO_TRIGANGLE(total_minutes / 2);
}

// 60 minutes = 360 degrees, so 1 degree = 1 minute (6 degrees per minute).
static int32_t minutes_angle(struct tm *t) {
  return DEG_TO_TRIGANGLE(t->tm_min * 6);
}

// 60 minutes = 360 degrees, so 1 degree = 1 minute (6 degrees per minute).
static int32_t seconds_angle(struct tm *t) {
  return DEG_TO_TRIGANGLE(t->tm_sec * 6);
}

static void update_hands(struct tm *t) {
  rot_bitmap_layer_set_angle(s_hour_hand_layer, hours_angle(t));
  rot_bitmap_layer_set_angle(s_minute_hand_layer, minutes_angle(t));
  rot_bitmap_layer_set_angle(s_second_hand_layer, seconds_angle(t));
}

static void update_display_text(struct tm *t) {
  text_layer_set_text(s_day_layer, day_string(t->tm_wday));
  text_layer_set_text(s_day_num_layer, day_num_string(t->tm_mday));
}

static bool tuple_bool(Tuple *t) {
  if (t->type == TUPLE_CSTRING) return ((const char *)t->value)[0] != '0';
  return t->value->int32 != 0;
}

static int tuple_int(Tuple *t) {
  if (t->type == TUPLE_CSTRING) return atoi((const char *)t->value);
  return (int)t->value->int32;
}

static void update_battery_display(void) {
  BatteryChargeState state = battery_state_service_peek();
  if (s_batt_display_pct) {
    const char *fmt = (state.is_charging || state.is_plugged) ? "+%d%%" : "%d%%";
    snprintf(s_batt_text_buf, sizeof(s_batt_text_buf), fmt, state.charge_percent);
    text_layer_set_text(s_batt_text_layer, s_batt_text_buf);
  } else {
    int frame = state.is_charging ? 10 : (10 - state.charge_percent / 10);
    if (s_batt_frame) gbitmap_destroy(s_batt_frame);
    s_batt_frame = gbitmap_create_as_sub_bitmap(s_batt_all, GRect(0, frame * 10, 20, 10));
    bitmap_layer_set_bitmap(s_batt_icon_layer, s_batt_frame);
  }
}

static void battery_callback(BatteryChargeState state) {
  update_battery_display();
}

static void update_weather_temp(void) {
  if (!s_weather_loaded) return;
  int t = s_units_fahrenheit ? (s_last_temp_c * 9 / 5 + 32) : (int)s_last_temp_c;
  snprintf(s_weather_temp_buf, sizeof(s_weather_temp_buf),
           s_units_fahrenheit ? "%d°F" : "%d°C", t);
  text_layer_set_text(s_weather_temp_layer, s_weather_temp_buf);
}

static void update_tick_subscription(void);

static void seconds_timer_callback(void *data) {
  s_seconds_timer = NULL;
  s_seconds_visible = false;
  layer_set_hidden(rot_bitmap_layer_get_layer(s_second_hand_layer), true);
  update_tick_subscription();
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  if (s_seconds_mode != SECONDS_FLICK) return;

  s_seconds_visible = true;
  layer_set_hidden(rot_bitmap_layer_get_layer(s_second_hand_layer), false);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  rot_bitmap_layer_set_angle(s_second_hand_layer, seconds_angle(t));

  update_tick_subscription();

  if (s_seconds_timer) app_timer_cancel(s_seconds_timer);
  s_seconds_timer = app_timer_register(BACKLIGHT_DURATION_MS, seconds_timer_callback, NULL);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (s_seconds_visible && (units_changed & SECOND_UNIT)) {
    rot_bitmap_layer_set_angle(s_second_hand_layer, seconds_angle(tick_time));
  }
  if (units_changed & MINUTE_UNIT) {
    rot_bitmap_layer_set_angle(s_minute_hand_layer, minutes_angle(tick_time));
    if (tick_time->tm_min % 2 == 0) {
      rot_bitmap_layer_set_angle(s_hour_hand_layer, hours_angle(tick_time));
    }
  }
  if (units_changed & HOUR_UNIT) {
    layer_set_hidden(s_dst_layer, tick_time->tm_isdst <= 0);
  }
  if (units_changed & DAY_UNIT) {
    update_display_text(tick_time);
  }
}

static void update_tick_subscription(void) {
  bool need_seconds = (s_seconds_mode == SECONDS_ALWAYS) ||
                      (s_seconds_mode == SECONDS_FLICK && s_seconds_visible);
  tick_timer_service_subscribe(need_seconds ? SECOND_UNIT : MINUTE_UNIT, tick_handler);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  Tuple *icon_t   = dict_find(iterator, MESSAGE_KEY_W_ICON);
  Tuple *temp_t   = dict_find(iterator, MESSAGE_KEY_W_TEMP);
  Tuple *units_t  = dict_find(iterator, MESSAGE_KEY_S_USE_FAHRENHEIT);
  Tuple *vib_bt_t   = dict_find(iterator, MESSAGE_KEY_S_VIB_BT);
  Tuple *batt_disp_t = dict_find(iterator, MESSAGE_KEY_S_BATT_SHOW_PCT);
  Tuple *show_sec_t  = dict_find(iterator, MESSAGE_KEY_S_SHOW_SECONDS);

  if (units_t) { s_units_fahrenheit = tuple_bool(units_t); update_weather_temp(); }
  if (vib_bt_t) { s_vib_bt = tuple_bool(vib_bt_t); }
  if (batt_disp_t) {
    s_batt_display_pct = tuple_bool(batt_disp_t);
    layer_set_hidden(bitmap_layer_get_layer(s_batt_icon_layer), s_batt_display_pct);
    layer_set_hidden(text_layer_get_layer(s_batt_text_layer), !s_batt_display_pct);
    update_battery_display();
  }
  if (show_sec_t) {
    s_seconds_mode = tuple_int(show_sec_t);
    if (s_seconds_timer) {
      app_timer_cancel(s_seconds_timer);
      s_seconds_timer = NULL;
    }
    s_seconds_visible = (s_seconds_mode == SECONDS_ALWAYS);
    layer_set_hidden(rot_bitmap_layer_get_layer(s_second_hand_layer), !s_seconds_visible);
    update_tick_subscription();
  }
  if (icon_t) {
    s_weather_icon_buf[0] = ((const char *)icon_t->value)[0];
    s_weather_icon_buf[1] = '\0';
    text_layer_set_text(s_weather_icon_layer, s_weather_icon_buf);
  }
  if (temp_t) {
    s_last_temp_c = (int16_t)temp_t->value->int32;
    s_weather_loaded = true;
    update_weather_temp();
  }
}

static void hands_pivot_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(CLOCK_CENTER_X, CLOCK_CENTER_Y), 3);
}

static RotBitmapLayer *create_hand_layer(Layer *parent, GPoint center,
                                          uint32_t resource_id, int size,
                                          GPoint pivot, GBitmap **out_bitmap) {
  *out_bitmap = gbitmap_create_with_resource(resource_id);
  RotBitmapLayer *rot_layer = rot_bitmap_layer_create(*out_bitmap);
  rot_bitmap_set_compositing_mode(rot_layer, GCompOpSet);
  int half = size / 2;
  layer_set_frame(rot_bitmap_layer_get_layer(rot_layer),
                  GRect(center.x - half, center.y - half, size, size));
  rot_bitmap_set_src_ic(rot_layer, pivot);
  layer_add_child(parent, rot_bitmap_layer_get_layer(rot_layer));
  return rot_layer;
}

static TextLayer *create_text_layer(Layer *parent, GRect text_box, GFont font, GTextAlignment alignment) {
  TextLayer *out_layer = text_layer_create(text_box);
  text_layer_set_background_color(out_layer, GColorClear);
  text_layer_set_text_color(out_layer, GColorLightGray);
  text_layer_set_font(out_layer, font);
  text_layer_set_text_alignment(out_layer, alignment);
  layer_add_child(parent, text_layer_get_layer(out_layer));
  return out_layer;
}

static void dst_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorDarkGray);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void bt_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorDarkGray);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void bluetooth_callback(bool connected) {
  layer_set_hidden(s_bt_layer, !connected);
  if (!connected && s_vib_bt) {
    vibes_double_pulse();
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = GPoint(CLOCK_CENTER_X, CLOCK_CENTER_Y);

  s_bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
  s_bg_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bg_layer));

  s_large_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DOTO_18));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DOTO_17));
  s_system_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

  s_day_layer = create_text_layer(window_layer, GRect(DAY_LAYER_X, DAY_LAYER_Y, DAY_LAYER_W, DAY_LAYER_H), s_large_font, GTextAlignmentCenter);
  text_layer_set_text_color(s_day_layer, GColorDarkGray);

  s_day_num_layer = create_text_layer(window_layer, GRect(DAY_NUM_LAYER_X, DAY_NUM_LAYER_Y, DAY_NUM_LAYER_W, DAY_NUM_LAYER_H), s_small_font, GTextAlignmentRight);
  text_layer_set_text_color(s_day_num_layer, GColorDarkGray);

  s_dst_layer = layer_create(GRect(DST_LAYER_X, DST_LAYER_Y, DST_LAYER_W, DST_LAYER_H));
  layer_set_update_proc(s_dst_layer, dst_layer_update_proc);
  layer_add_child(window_layer, s_dst_layer);

  s_bt_layer = layer_create(GRect(BT_LAYER_X, BT_LAYER_Y, BT_LAYER_W, BT_LAYER_H));
  layer_set_update_proc(s_bt_layer, bt_layer_update_proc);
  layer_add_child(window_layer, s_bt_layer);
  layer_set_hidden(s_bt_layer, !bluetooth_connection_service_peek());

  s_batt_all = gbitmap_create_with_resource(RESOURCE_ID_BATTERY);
  s_batt_frame = NULL;
  s_batt_icon_layer = bitmap_layer_create(GRect(BATT_ICON_X, BATT_ICON_Y, BATT_ICON_W, BATT_ICON_H));
  bitmap_layer_set_background_color(s_batt_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_batt_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_batt_icon_layer));

  s_batt_text_layer = create_text_layer(window_layer,
    GRect(BATT_TEXT_X, BATT_TEXT_Y, BATT_TEXT_W, BATT_TEXT_H),
    s_system_font, GTextAlignmentRight);
  text_layer_set_text_color(s_batt_text_layer, GColorWhite);
  text_layer_set_text(s_batt_text_layer, s_batt_text_buf);
  layer_set_hidden(bitmap_layer_get_layer(s_batt_icon_layer), s_batt_display_pct);
  layer_set_hidden(text_layer_get_layer(s_batt_text_layer), !s_batt_display_pct);
  update_battery_display();

  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_WEATHER_ICON_34));
  s_weather_icon_layer = create_text_layer(window_layer,
    GRect(WEATHER_ICON_X, WEATHER_ICON_Y, WEATHER_ICON_W, WEATHER_ICON_H),
    s_weather_font, GTextAlignmentRight);
  text_layer_set_text_color(s_weather_icon_layer, GColorWhite);
  text_layer_set_text(s_weather_icon_layer, s_weather_icon_buf);

  s_weather_temp_layer = create_text_layer(window_layer,
    GRect(WEATHER_TEMP_X, WEATHER_TEMP_Y, WEATHER_TEMP_W, WEATHER_TEMP_H),
    s_system_font, GTextAlignmentLeft);
  text_layer_set_text_color(s_weather_temp_layer, GColorWhite);
  text_layer_set_text(s_weather_temp_layer, s_weather_temp_buf);

  s_minute_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_MINUTE_HAND,
                                           MINUTE_LAYER_SIZE, GPoint(MINUTE_PIVOT_X, MINUTE_PIVOT_Y),
                                           &s_minute_hand_bitmap);
  s_hour_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_HOUR_HAND,
                                         HOUR_LAYER_SIZE, GPoint(HOUR_PIVOT_X, HOUR_PIVOT_Y),
                                         &s_hour_hand_bitmap);
  s_second_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_SECOND_HAND,
                                           SECOND_LAYER_SIZE, GPoint(SECOND_PIVOT_X, SECOND_PIVOT_Y),
                                           &s_second_hand_bitmap);

  s_hands_pivot_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_pivot_layer, hands_pivot_layer_update_proc);
  layer_add_child(window_layer, s_hands_pivot_layer);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_hands(t);
  update_display_text(t);
  layer_set_hidden(s_dst_layer, t->tm_isdst <= 0);
}

static void prv_window_unload(Window *window) {
  bitmap_layer_destroy(s_batt_icon_layer);
  text_layer_destroy(s_batt_text_layer);
  text_layer_destroy(s_weather_icon_layer);
  text_layer_destroy(s_weather_temp_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_day_num_layer);
  layer_destroy(s_dst_layer);
  layer_destroy(s_bt_layer);
  layer_destroy(s_hands_pivot_layer);
  rot_bitmap_layer_destroy(s_second_hand_layer);
  rot_bitmap_layer_destroy(s_minute_hand_layer);
  rot_bitmap_layer_destroy(s_hour_hand_layer);
  bitmap_layer_destroy(s_bg_layer);
  if (s_batt_frame) gbitmap_destroy(s_batt_frame);
  gbitmap_destroy(s_batt_all);
  gbitmap_destroy(s_second_hand_bitmap);
  gbitmap_destroy(s_minute_hand_bitmap);
  gbitmap_destroy(s_hour_hand_bitmap);
  gbitmap_destroy(s_bg_bitmap);
  fonts_unload_custom_font(s_weather_font);
  fonts_unload_custom_font(s_large_font);
  fonts_unload_custom_font(s_small_font);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(256, 64);
  battery_state_service_subscribe(battery_callback);
  bluetooth_connection_service_subscribe(bluetooth_callback);
  accel_tap_service_subscribe(accel_tap_handler);
  update_tick_subscription();
  window_stack_push(s_window, true);
}

static void prv_deinit(void) {
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
