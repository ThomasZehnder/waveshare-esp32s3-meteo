#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *connect_status_label;
lv_obj_t *meteo_outside_label; 
lv_obj_t *meteo_inside_label; 


void getMeteoString(char *buf, size_t bufsize, const char* prefix, const meteo_t &meteo)
{
    snprintf(buf, bufsize, "%s DeviceName: %s, Temp: %.2f, Hum: %.2f", prefix, meteo.deviceName.c_str(), meteo.temperature, meteo.humidity);
}


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

    //show Meteo Outside status
    char buf[128];
    getMeteoString(buf, sizeof(buf), "Meteo Outside" , Asset.outside);
    meteo_outside_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text(meteo_outside_label, buf);
    lv_obj_align(meteo_outside_label, LV_ALIGN_TOP_LEFT, 0, 40);    

    //show Meteo Inside status
    getMeteoString(buf, sizeof(buf), "Meteo Inside" , Asset.inside);
    meteo_inside_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text(meteo_inside_label, buf);
    lv_obj_align(meteo_inside_label, LV_ALIGN_TOP_LEFT, 0, 60);    

 

}

void ui_Service_Screen_update(void)
{
    lv_label_set_text_fmt(connect_status_label, "IP Address: %s, WiFi Connected: %s, MQTT Connected: %s", Asset.ipAddr.c_str(), Asset.wifiConnected ? "Yes" : "No", Asset.mqttConnected ? "Yes" : "No");
    char buf[128];
    getMeteoString(buf, sizeof(buf), "Meteo Outside" , Asset.outside);
    lv_label_set_text(meteo_outside_label, buf);
    getMeteoString(buf, sizeof(buf), "Meteo Inside" , Asset.inside);
    lv_label_set_text(meteo_inside_label, buf);

}   