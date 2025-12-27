#include <Arduino.h>
#include <Wire.h>
#include "diagnostics.h"
#include "touch.h"

void i2c_scanner() {
    Serial.println("\n=== I2C Scanner ===");
    Serial.println("Scanning I2C bus...");

    int nDevices = 0;
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf("Device found at address 0x%02X\n", address);
            nDevices++;
        }
    }

    if (nDevices == 0) {
        Serial.println("No I2C devices found");
    } else {
        Serial.printf("Found %d I2C device(s)\n", nDevices);
    }
    Serial.println("===================\n");
}

void manual_touch_test() {
    Serial.println("\n=== Manual GT911 Touch Test ===");

    for (int i = 0; i < 10; i++) {
        uint8_t status = 0;
        bool read_ok = gt911_read_reg(0x814E, &status, 1);

        Serial.printf("Test %d: Status read %s, value=0x%02X, ready=%s, points=%d\n",
                      i + 1,
                      read_ok ? "OK" : "FAIL",
                      status,
                      (status & 0x80) ? "YES" : "NO",
                      status & 0x0F);

        if (status & 0x80) {
            Serial.println("✅ Touch detected during test!");
            uint8_t touch_data[8] = {0};
            if (gt911_read_reg(0x8150, touch_data, 8)) {
                uint16_t x = (touch_data[1] << 8) | touch_data[0];
                uint16_t y = (touch_data[3] << 8) | touch_data[2];
                Serial.printf("Touch coordinates: x=%d, y=%d\n", x, y);
            }

            uint8_t clear = 0;
            gt911_write_reg(0x814E, &clear, 1);
        }

        delay(100);
    }

    Serial.println("=== End Manual Test ===\n");
}
