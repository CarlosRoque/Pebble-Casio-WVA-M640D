#pragma once

// Clock face center on screen (tune to match background image)
#define CLOCK_CENTER_X 100
#define CLOCK_CENTER_Y 111

// Hour hand: 8x67px, pivot 49px from top
#define HOUR_PIVOT_X 5
#define HOUR_PIVOT_Y 45
#define HOUR_LAYER_SIZE 90  // 2 * ceil(max rotation radius ~49px) + margin

// Minute hand: 6x86px, pivot 64px from top
#define MINUTE_PIVOT_X 4
#define MINUTE_PIVOT_Y 64
#define MINUTE_LAYER_SIZE 128  // 2 * ceil(max rotation radius ~67px) + margin

// Second hand: 6x93px, pivot 71px from top
#define SECOND_PIVOT_X 3
#define SECOND_PIVOT_Y 71
#define SECOND_LAYER_SIZE 142  // 2 * ceil(max rotation radius ~67px) + margin

// Weather layer layout
#define WEATHER_ICON_X  150
#define WEATHER_ICON_Y  185
#define WEATHER_ICON_W   40
#define WEATHER_ICON_H   45

#define WEATHER_TEMP_X   10
#define WEATHER_TEMP_Y  195
#define WEATHER_TEMP_W   70
#define WEATHER_TEMP_H   22

// Date and status indicator layout
#define DAY_LAYER_X     60
#define DAY_LAYER_Y    142
#define DAY_LAYER_W     45
#define DAY_LAYER_H     40

#define DAY_NUM_LAYER_X 105
#define DAY_NUM_LAYER_Y 146
#define DAY_NUM_LAYER_W  25
#define DAY_NUM_LAYER_H  20

#define DST_LAYER_X    125
#define DST_LAYER_Y    146
#define DST_LAYER_W     12
#define DST_LAYER_H      3

#define BT_LAYER_X     105
#define BT_LAYER_Y     146
#define BT_LAYER_W      12
#define BT_LAYER_H       3

// Battery indicator layout
#define BATT_ICON_X    170
#define BATT_ICON_Y     13
#define BATT_ICON_W     20
#define BATT_ICON_H     10

#define BATT_TEXT_X    145
#define BATT_TEXT_Y      3
#define BATT_TEXT_W     50
#define BATT_TEXT_H     18
