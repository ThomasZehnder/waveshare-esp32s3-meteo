#pragma once

#include <stdint.h>
#include <lvgl.h>

// Pin accessors for external re-init
int touch_get_sda();
int touch_get_scl();
uint8_t touch_get_default_addr();

// Initialize GT911 and set screen resolution for validation
bool touch_init(uint16_t h_res, uint16_t v_res);

// LVGL input driver callback
void touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

// Low-level GT911 register helpers (used by diagnostics)
bool gt911_read_reg(uint16_t reg, uint8_t *data, uint8_t len);
void gt911_write_reg(uint16_t reg, uint8_t *data, uint8_t len);
