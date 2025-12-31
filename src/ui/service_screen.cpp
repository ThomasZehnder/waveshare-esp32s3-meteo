#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *connect_status_label;
lv_obj_t *meteo_outside_label; 
lv_obj_t *meteo_inside_label; 




void ui_Service_Screen_init(void)
{
    UI_Screens.Service_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Service_Screen);

    lv_obj_t *screen_service_ui_label = lv_label_create(UI_Screens.Service_Screen);
    lv_label_set_text_fmt(screen_service_ui_label, "Service Screen : Device ID: %s", Asset.deviceId.c_str());
    lv_obj_align(screen_service_ui_label, LV_ALIGN_TOP_LEFT, 0, 0);

    //show WiFi IP address
    connect_status_label = lv_label_create(UI_Screens.Service_Screen);    
    lv_label_set_text_fmt(connect_status_label, "IP Address: %s", Asset.ipAddr.c_str());
    lv_obj_align(connect_status_label, LV_ALIGN_TOP_LEFT, 0, 20);

    //show WiFi status
    char buf[128];
    snprintf(buf, sizeof(buf), "Meteo Outside: DeviceName: %s, Temp: %.2f, Hum: %.2f", Asset.outside.deviceName.c_str(), Asset.outside.temperature, Asset.outside.humidity);
    meteo_outside_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text(meteo_outside_label, buf);
    lv_obj_align(meteo_outside_label, LV_ALIGN_TOP_LEFT, 0, 40);    

    //show MQTT status
    char buf_inside[128];
    snprintf(buf_inside, sizeof(buf_inside), "Meteo Inside: DeviceName: %s, Temp: %.2f, Hum: %.2f", Asset.inside.deviceName.c_str(), Asset.inside.temperature, Asset.inside.humidity);
    meteo_inside_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text(meteo_inside_label, buf_inside);
    lv_obj_align(meteo_inside_label, LV_ALIGN_TOP_LEFT, 0, 60);    

}

void ui_Service_Screen_update(void)
{
    lv_label_set_text_fmt(connect_status_label, "IP Address: %s, WiFi Connected: %s, MQTT Connected: %s", Asset.ipAddr.c_str(), Asset.wifiConnected ? "Yes" : "No", Asset.mqttConnected ? "Yes" : "No");
    char buf_out[128];
    snprintf(buf_out, sizeof(buf_out), "Meteo Outside: DeviceName: %s, Temp: %.2f, Hum: %.2f", Asset.outside.deviceName.c_str(), Asset.outside.temperature, Asset.outside.humidity);
    lv_label_set_text(meteo_outside_label, buf_out);
    char buf_in[128];
    snprintf(buf_in, sizeof(buf_in), "Meteo Inside: DeviceName: %s, Temp: %.2f, Hum: %.2f", Asset.inside.deviceName.c_str(), Asset.inside.temperature, Asset.inside.humidity);
    lv_label_set_text(meteo_inside_label, buf_in);
}   