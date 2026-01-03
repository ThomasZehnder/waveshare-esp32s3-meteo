#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include "diagnostics.h"
#include "touch.h"
#include "BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h"

// GT911 Touch Configuration - primary and fallback
static constexpr int TOUCH_SDA = 8;
static constexpr int TOUCH_SCL = 9;
static constexpr int TOUCH_INT = 4;
static constexpr int TOUCH_RST = -1; // May be controlled by expander
static constexpr uint8_t GT911_I2C_ADDR_1 = 0x5D;
static constexpr uint8_t GT911_I2C_ADDR_2 = 0x14;

// GT911 Register definitions
static constexpr uint16_t GT911_STATUS_REG = 0x814E;
static constexpr uint16_t GT911_POINT1_REG = 0x814F;
static constexpr uint16_t GT911_CONFIG_REG = 0x8047;
static constexpr uint16_t GT911_PRODUCT_ID = 0x8140;

static uint8_t gt911_i2c_addr = GT911_I2C_ADDR_1;
static uint16_t screen_h_res = ESP_PANEL_BOARD_WIDTH;
static uint16_t screen_v_res = ESP_PANEL_BOARD_HEIGHT;

int touch_get_sda() { return TOUCH_SDA; }
int touch_get_scl() { return TOUCH_SCL; }
uint8_t touch_get_default_addr() { return GT911_I2C_ADDR_1; }

void gt911_write_reg(uint16_t reg, uint8_t *data, uint8_t len)
{
    Wire.beginTransmission(gt911_i2c_addr);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    for (uint8_t i = 0; i < len; i++)
    {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}

bool gt911_read_reg(uint16_t reg, uint8_t *data, uint8_t len)
{
    Wire.beginTransmission(gt911_i2c_addr);
    Wire.write((reg >> 8) & 0xFF);
    Wire.write(reg & 0xFF);
    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    uint8_t received = Wire.requestFrom((uint8_t)gt911_i2c_addr, (uint8_t)len);
    if (received != len)
    {
        return false;
    }

    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = Wire.read();
    }
    return true;
}

static bool gt911_init_internal()
{
    // Try different pin configurations
    int8_t pin_configs[][4] = {
        {TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST}, // Primary
        {19, 20, 21, 38},                             // Spotpear variant
        {6, 7, 5, -1},                                // Alternative 1
        {17, 18, 16, -1}                              // Alternative 2
    };

    for (int config = 0; config < 4; config++)
    {
        uint8_t sda = pin_configs[config][0];
        uint8_t scl = pin_configs[config][1];
        uint8_t int_pin = pin_configs[config][2];
        int8_t rst_pin = pin_configs[config][3];

        Serial.printf("\n--- Trying pin config %d: SDA=%d, SCL=%d, INT=%d, RST=%d ---\n",
                      config + 1, sda, scl, int_pin, rst_pin);

        if (rst_pin >= 0)
        {
            pinMode(rst_pin, OUTPUT);
            digitalWrite(rst_pin, LOW);
            delay(10);
            digitalWrite(rst_pin, HIGH);
            delay(50);
        }

        if (int_pin >= 0)
        {
            pinMode(int_pin, INPUT_PULLUP);
        }

        Wire.end();
        Wire.begin(sda, scl, 400000);
        delay(100);

        // i2c_scanner();

        uint8_t addresses[] = {GT911_I2C_ADDR_1, GT911_I2C_ADDR_2};
        for (int addr_idx = 0; addr_idx < 2; addr_idx++)
        {
            gt911_i2c_addr = addresses[addr_idx];
            uint8_t test_data[4] = {0};

            Serial.printf("Trying GT911 address 0x%02X...\n", gt911_i2c_addr);
            if (gt911_read_reg(GT911_PRODUCT_ID, test_data, 4))
            {
                Serial.printf("Product ID: %02X %02X %02X %02X (chars: %c%c%c%c)\n",
                              test_data[0], test_data[1], test_data[2], test_data[3],
                              test_data[0], test_data[1], test_data[2], test_data[3]);

                if (test_data[0] == '9' && test_data[1] == '1' && test_data[2] == '1')
                {
                    Serial.printf("✅ GT911 found! SDA=%d, SCL=%d, Address=0x%02X\n", sda, scl, gt911_i2c_addr);
                    return true;
                }
            }
        }
    }

    Serial.println("❌ GT911 not found with any pin configuration");
    return false;
}

bool touch_init(uint16_t h_res, uint16_t v_res)
{
    screen_h_res = h_res;
    screen_v_res = v_res;
    return gt911_init_internal();
}

void touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    static uint16_t last_x = 400; // Start at screen center
    static uint16_t last_y = 240;
    static bool was_pressed = false;
    static uint32_t read_counter = 0;
    static uint32_t last_report_time = 0;
    static uint8_t last_status = 0x80;

    /*
    read_counter++;

    // Debug LVGL touch reads every 1000 calls (about 5 seconds at 200Hz)
    if (millis() - last_report_time >= 5000) {
        Serial.printf("🎯 touchpad_read called %lu times in 5 seconds\n", read_counter);
        read_counter = 0;
        last_report_time = millis();
    }
    */

    // Throttle GT911 reads to prevent I2C timing issues (max 20Hz = 50ms interval)
    static uint32_t last_gt911_read = 0;
    if (millis() - last_gt911_read < 50)
    {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = was_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
        return;
    }
    last_gt911_read = millis();

    uint8_t status = 0x80;
    bool current_touch_detected = false;
    uint16_t x = last_x, y = last_y;

    if (gt911_read_reg(GT911_STATUS_REG, &status, 1))
    {
        bool ready = (status & 0x80) != 0;
        uint8_t points = status & 0x0F;

        if (ready && points > 0)
        {
            current_touch_detected = true;

            if (status != last_status || !was_pressed)
            {
                Serial.printf("📍 Status change detected: 0x%02X -> 0x%02X, reading coordinates...\n", last_status, status);

                bool coord_read_success = false;
                for (int attempt = 0; attempt < 3 && !coord_read_success; attempt++)
                {
                    delay(2);

                    uint8_t coords[8] = {0};
                    if (gt911_read_reg(GT911_POINT1_REG, coords, sizeof(coords)))
                    {
                        uint16_t new_x = (uint16_t)(coords[2] << 8) | coords[1];
                        uint16_t new_y = (uint16_t)(coords[4] << 8) | coords[3];

                        if (new_x <= screen_h_res && new_y <= screen_v_res)
                        {
                            x = new_x;
                            y = new_y;
                            coord_read_success = true;

                            Serial.printf("🎉 VALID Touch: x=%d, y=%d (attempt %d, status=0x%02X, tid=%d)\n", x, y, attempt + 1, status, coords[0]);
                            break;
                        }
                        else
                        {
                            Serial.printf("🔄 Retry %d: Invalid coords x=%d, y=%d (raw: %02X %02X %02X %02X %02X %02X %02X %02X)\n",
                                          attempt + 1, new_x, new_y,
                                          coords[0], coords[1], coords[2], coords[3], coords[4], coords[5], coords[6], coords[7]);
                        }
                    }
                    delay(1);
                }

                if (!coord_read_success)
                {
                    Serial.printf("❌ Failed to read valid coordinates after 3 attempts (status=0x%02X)\n", status);
                }
            }
            else
            {
                x = last_x;
                y = last_y;
            }
        }
        else if (was_pressed && (points == 0 || !ready))
        {
            Serial.printf("👆 Touch RELEASED (status=0x%02X)\n", status);
        }

        last_status = status;

        uint8_t clear = 0x00;
        gt911_write_reg(GT911_STATUS_REG, &clear, 1);
    }

    if (current_touch_detected)
    {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
        last_x = x;
        last_y = y;
        was_pressed = true;
    }
    else
    {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_RELEASED;
        was_pressed = false;
    }
}
