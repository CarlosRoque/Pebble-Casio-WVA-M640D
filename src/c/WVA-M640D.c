#include <pebble.h>

// Clock face center on screen (tune to match background image)
#define CLOCK_CENTER_X 100
#define CLOCK_CENTER_Y 112

// Hour hand: 8x67px, pivot 49px from top
#define HOUR_PIVOT_X 4
#define HOUR_PIVOT_Y 49
#define HOUR_LAYER_SIZE 98  // 2 * ceil(max rotation radius ~49px) + margin

// Minute hand: 6x86px, pivot 64px from top
#define MINUTE_PIVOT_X 3
#define MINUTE_PIVOT_Y 64
#define MINUTE_LAYER_SIZE 128  // 2 * ceil(max rotation radius ~67px) + margin

// Second hand: 6x93px, pivot 71px from top
#define SECOND_PIVOT_X 3
#define SECOND_PIVOT_Y 71
#define SECOND_LAYER_SIZE 142  // 2 * ceil(max rotation radius ~67px) + margin

static int s_last_wday = -1;

static Window *s_window;
static Layer *s_debug_layer;
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
static TextLayer *s_day_num_layer;
static GFont s_day_num_font;
static Layer *s_dst_layer;

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

static void update_text(struct tm *t) {
  text_layer_set_text(s_day_layer, day_string(t->tm_wday));
  text_layer_set_text(s_day_num_layer, day_num_string(t->tm_mday));
}

static void debug_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(CLOCK_CENTER_X, CLOCK_CENTER_Y), 3);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & SECOND_UNIT) {
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
    update_text(tick_time);
  }
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

static TextLayer *create_text_layer(Layer *parent, GRect text_box, GFont font, uint32_t aligment) {
  TextLayer *out_layer = text_layer_create(text_box);
  text_layer_set_background_color(out_layer, GColorClear);
  text_layer_set_text_color(out_layer, GColorLightGray);
  text_layer_set_font(out_layer, font);
  text_layer_set_text_alignment(out_layer, aligment);
  layer_add_child(parent, text_layer_get_layer(out_layer));
  return out_layer;
}

static void dst_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorLightGray);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
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
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DOTO_16));

  s_day_layer = create_text_layer(window_layer, GRect(60, 142, 45, 40), s_large_font, GTextAlignmentCenter);

  s_day_num_layer = create_text_layer(window_layer, GRect(105, 145, 25, 20), s_small_font, GTextAlignmentRight);

  s_dst_layer = layer_create(GRect(125, 146, 12, 3));
  layer_set_update_proc(s_dst_layer, dst_layer_update_proc);
  layer_add_child(window_layer, s_dst_layer);

  s_hour_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_HOUR_HAND,
                                         HOUR_LAYER_SIZE, GPoint(HOUR_PIVOT_X, HOUR_PIVOT_Y),
                                         &s_hour_hand_bitmap);
  s_minute_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_MINUTE_HAND,
                                           MINUTE_LAYER_SIZE, GPoint(MINUTE_PIVOT_X, MINUTE_PIVOT_Y),
                                           &s_minute_hand_bitmap);
  s_second_hand_layer = create_hand_layer(window_layer, center, RESOURCE_ID_SECOND_HAND,
                                           SECOND_LAYER_SIZE, GPoint(SECOND_PIVOT_X, SECOND_PIVOT_Y),
                                           &s_second_hand_bitmap);

  s_debug_layer = layer_create(bounds);
  layer_set_update_proc(s_debug_layer, debug_layer_update_proc);
  layer_add_child(window_layer, s_debug_layer);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_hands(t);
  update_text(t);
  layer_set_hidden(s_dst_layer, t->tm_isdst <= 0);
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_day_layer);
  fonts_unload_custom_font(s_large_font);
  fonts_unload_custom_font(s_small_font);
  text_layer_destroy(s_day_num_layer);
  fonts_unload_custom_font(s_day_num_font);
  layer_destroy(s_debug_layer);
  rot_bitmap_layer_destroy(s_second_hand_layer);
  gbitmap_destroy(s_second_hand_bitmap);
  rot_bitmap_layer_destroy(s_minute_hand_layer);
  gbitmap_destroy(s_minute_hand_bitmap);
  rot_bitmap_layer_destroy(s_hour_hand_layer);
  gbitmap_destroy(s_hour_hand_bitmap);
  bitmap_layer_destroy(s_bg_layer);
  gbitmap_destroy(s_bg_bitmap);
  layer_destroy(s_dst_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  window_stack_push(s_window, true);
}

static void prv_deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
