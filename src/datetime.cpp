#include "datetime.h"
#include "asset.h"
#include <time.h>

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 0;  // UTC
const int daylightOffset_sec = 0;

void setupNTP() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

void updateDatetime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    Asset.datetime.year = timeinfo.tm_year + 1900;
    Asset.datetime.month = timeinfo.tm_mon + 1;
    Asset.datetime.day = timeinfo.tm_mday;
    Asset.datetime.hour = timeinfo.tm_hour;
    Asset.datetime.minute = timeinfo.tm_min;
    Asset.datetime.second = timeinfo.tm_sec;

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    Asset.datetime.formatted = String(buffer);
}