#include "ui.h"
#include <Arduino.h>
#include "asset.h"


lv_obj_t *ui_1_label1;
lv_obj_t *ui_1_label2;





void ui_Local_screen_init(void)
{
    UI_Screens.Local_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Local_Screen);

    ui_1_label1 = lv_label_create(UI_Screens.Local_Screen);
    lv_label_set_text(ui_1_label1, "Inside Values ");
    lv_obj_align(ui_1_label1, LV_ALIGN_TOP_LEFT, 0, 0);
    // lv_obj_set_style_text_color(ui_label8, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_1_label1, &lv_font_montserrat_30, DEFAULT_SELECTOR);

    ui_1_label2 = lv_label_create(UI_Screens.Local_Screen);
    lv_label_set_text(ui_1_label2, "Outside Values ");
    lv_obj_align(ui_1_label2, LV_ALIGN_TOP_LEFT, 0, 40);
    // lv_obj_set_style_text_color(ui_label8, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_1_label2, &lv_font_montserrat_30, DEFAULT_SELECTOR);
}

void ui_Local_screen_update(void)
{
    char buf[128];
    getMeteoString(buf, sizeof(buf), "Meteo Inside" , Asset.inside);
    lv_label_set_text(ui_1_label1, buf);    
    getMeteoString(buf, sizeof(buf), "Meteo Outside" , Asset.outside);
    lv_label_set_text(ui_1_label2, buf);

}