#pragma once

#include <lvgl.h>
#include "esp_lcd_panel_rgb.h"
#include "BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h"

// LCD panel configuration (Waveshare ESP32-S3 4.3" RGB)
#define LCD_H_RES ESP_PANEL_BOARD_WIDTH
#define LCD_V_RES ESP_PANEL_BOARD_HEIGHT
#define LCD_BL 2
#define LCD_PCLK 7
#define LCD_DE 5
#define LCD_VSYNC 3
#define LCD_HSYNC 46
// RGB565 data pins
#define LCD_DATA0 1
#define LCD_DATA1 2
#define LCD_DATA2 42
#define LCD_DATA3 41
#define LCD_DATA4 40
#define LCD_DATA5 39
#define LCD_DATA6 0
#define LCD_DATA7 45
#define LCD_DATA8 48
#define LCD_DATA9 47
#define LCD_DATA10 21
#define LCD_DATA11 14
#define LCD_DATA12 38
#define LCD_DATA13 18
#define LCD_DATA14 17
#define LCD_DATA15 10

// Initialize panel, LVGL draw buffer, and register LVGL display driver
bool lcd_init_display(lv_disp_draw_buf_t *draw_buf, lv_color_t **buf1, lv_color_t **buf2);

// Draw basic RGB test pattern
void lcd_draw_test_pattern();

// LVGL flush callback bound to the registered panel
void lcd_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

// Panel handle accessor (for advanced use)
esp_lcd_panel_handle_t lcd_get_panel_handle();
