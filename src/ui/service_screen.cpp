#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *ip_label;
lv_obj_t *wifi_status_label;

void ui_Service_Screen_init(void)
{
    UI_Screens.Service_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Service_Screen);

    lv_obj_t *screen_service_ui_label = lv_label_create(UI_Screens.Service_Screen);
    lv_label_set_text(screen_service_ui_label, "Service Screen ...");
    lv_obj_align(screen_service_ui_label, LV_ALIGN_TOP_LEFT, 0, 0);

    //show WiFi IP address
    ip_label = lv_label_create(UI_Screens.Service_Screen);    
    lv_label_set_text_fmt(ip_label, "IP Address: %s", Asset.ipAddr.c_str());
    lv_obj_align(ip_label, LV_ALIGN_TOP_LEFT, 0, 20);

    //show WiFi status
    wifi_status_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text_fmt(wifi_status_label, "WiFi Connected: %s", Asset.wifiConnected ? "Yes" : "No");
    lv_obj_align(wifi_status_label, LV_ALIGN_TOP_LEFT, 0, 40);    

    //show MQTT status
    wifi_status_label = lv_label_create(UI_Screens.Service_Screen);   
    lv_label_set_text_fmt(wifi_status_label, "MQTT Connected: %s", Asset.mqttConnected ? "Yes" : "No");
    lv_obj_align(wifi_status_label, LV_ALIGN_TOP_LEFT, 0, 60);    

}

void ui_Service_Screen_update(void)
{
    lv_label_set_text_fmt(ip_label, "IP Address: %s", Asset.ipAddr.c_str());
    lv_label_set_text_fmt(wifi_status_label, "WiFi Connected: %s", Asset.wifiConnected ? "Yes" : "No");
    lv_label_set_text_fmt(wifi_status_label, "MQTT Connected: %s", Asset.mqttConnected ? "Yes" : "No");
}   