#include <Arduino.h>
#include <lvgl.h>
#include "esp_err.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_ops.h"
#include "lcd.h"

static esp_lcd_panel_handle_t s_panel_handle = nullptr;
static bool s_disp_registered = false;

esp_lcd_panel_handle_t lcd_get_panel_handle() {
    return s_panel_handle;
}

void lcd_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    if (!s_panel_handle) {
        lv_disp_flush_ready(drv);
        return;
    }
    esp_lcd_panel_draw_bitmap(s_panel_handle,
                              area->x1, area->y1,
                              area->x2 + 1, area->y2 + 1,
                              color_map);
    lv_disp_flush_ready(drv);
}

bool lcd_init_display(lv_disp_draw_buf_t *draw_buf, lv_color_t **buf1, lv_color_t **buf2)
{
    // Allocate full-screen buffers for double buffering to reduce flicker
    size_t buf_pixels = LCD_H_RES * LCD_V_RES;
    *buf1 = (lv_color_t *)heap_caps_malloc(buf_pixels * sizeof(lv_color_t),
                                           MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    *buf2 = (lv_color_t *)heap_caps_malloc(buf_pixels * sizeof(lv_color_t),
                                           MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!*buf1 || !*buf2) {
        Serial.println("❌ Failed to allocate LVGL draw buffers");
        return false;
    }
    lv_disp_draw_buf_init(draw_buf, *buf1, *buf2, buf_pixels);

    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings = {
            .pclk_hz = 16000000, // moderate pixel clock
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            .hsync_pulse_width = 30,
            .hsync_back_porch = 16,
            .hsync_front_porch = 210,
            .vsync_pulse_width = 13,
            .vsync_back_porch = 10,
            .vsync_front_porch = 22,
        },
        .data_width = 16,
        .sram_trans_align = 64,
        .psram_trans_align = 64,
        .hsync_gpio_num = LCD_HSYNC,
        .vsync_gpio_num = LCD_VSYNC,
        .de_gpio_num = LCD_DE,
        .pclk_gpio_num = LCD_PCLK,
        .data_gpio_nums = {LCD_DATA0, LCD_DATA1, LCD_DATA2, LCD_DATA3, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7,
                           LCD_DATA8, LCD_DATA9, LCD_DATA10, LCD_DATA11, LCD_DATA12, LCD_DATA13, LCD_DATA14, LCD_DATA15},
        .disp_gpio_num = -1,
    };

    panel_config.flags.fb_in_psram = 1;

    esp_err_t err = esp_lcd_new_rgb_panel(&panel_config, &s_panel_handle);
    if (err != ESP_OK) {
        Serial.printf("esp_lcd_new_rgb_panel failed: %s\n", esp_err_to_name(err));
        return false;
    }

    err = esp_lcd_panel_init(s_panel_handle);
    if (err != ESP_OK) {
        Serial.printf("esp_lcd_panel_init failed: %s\n", esp_err_to_name(err));
        return false;
    }

    // Register LVGL display driver once
    if (!s_disp_registered) {
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = LCD_H_RES;
        disp_drv.ver_res = LCD_V_RES;
        disp_drv.flush_cb = lcd_flush_cb;
        disp_drv.draw_buf = draw_buf;
        lv_disp_drv_register(&disp_drv);
        s_disp_registered = true;
    }

    Serial.println("✅ LCD initialized and LVGL driver registered");
    return true;
}

void lcd_draw_test_pattern()
{
    if (!s_panel_handle) {
        Serial.println("❌ Cannot draw test pattern, panel not initialized");
        return;
    }

    lv_color_t *test_buf = (lv_color_t *)heap_caps_malloc(LCD_H_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    if (!test_buf) {
        Serial.println("❌ Failed to allocate test buffer");
        return;
    }

    // Red stripe
    for (int i = 0; i < LCD_H_RES; i++) test_buf[i] = lv_color_make(255, 0, 0);
    for (int y = 0; y < LCD_V_RES / 3; y++) esp_lcd_panel_draw_bitmap(s_panel_handle, 0, y, LCD_H_RES, y + 1, test_buf);

    // Green stripe
    for (int i = 0; i < LCD_H_RES; i++) test_buf[i] = lv_color_make(0, 255, 0);
    for (int y = LCD_V_RES / 3; y < (LCD_V_RES * 2) / 3; y++) esp_lcd_panel_draw_bitmap(s_panel_handle, 0, y, LCD_H_RES, y + 1, test_buf);

    // Blue stripe
    for (int i = 0; i < LCD_H_RES; i++) test_buf[i] = lv_color_make(0, 0, 255);
    for (int y = (LCD_V_RES * 2) / 3; y < LCD_V_RES; y++) esp_lcd_panel_draw_bitmap(s_panel_handle, 0, y, LCD_H_RES, y + 1, test_buf);

    heap_caps_free(test_buf);
    Serial.println("Blue stripe drawn - RGB test complete");
}
