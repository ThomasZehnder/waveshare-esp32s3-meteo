#include "ui.h"
#include <Arduino.h>

lv_obj_t *ui_2_table;

void updateTableWithForecastData(lv_obj_t *table)
{
    Serial.println("Updating forecast table UI");
    // Set header row
    lv_table_set_cell_value(table, 0, 0, "Day");
    lv_table_set_cell_value(table, 0, 1, "Min Temp (°C)");
    lv_table_set_cell_value(table, 0, 2, "Max Temp (°C)");
    lv_table_set_cell_value(table, 0, 3, "Precipitation (mm)");
    lv_table_set_cell_value(table, 0, 4, "Wind Speed (km/h)");

    // Fill table with forecast data
    for (int i = 0; i < NUM_FORECAST_DAYS; i++)
    {
        lv_table_set_cell_value(table, i + 1, 0, Asset.forecast.time[i].c_str());
        lv_table_set_cell_value(table, i + 1, 1, String(Asset.forecast.temperature_2m_min[i], 2).c_str());
        lv_table_set_cell_value(table, i + 1, 2, String(Asset.forecast.temperature_2m_max[i], 2).c_str());
        lv_table_set_cell_value(table, i + 1, 3, String(Asset.forecast.precipitation_sum[i], 2).c_str());
        lv_table_set_cell_value(table, i + 1, 4, String(Asset.forecast.wind_speed_10m_max[i], 2).c_str());
    }
}

void ui_Forecast_screen_init(void)
{
    UI_Screens.Forecast_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Forecast_Screen);

    lv_obj_t *ui_2_label1 = lv_label_create(UI_Screens.Forecast_Screen);
    lv_label_set_text(ui_2_label1, "Forcast for 9500-Wil");
    lv_obj_align(ui_2_label1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_font(ui_2_label1, &lv_font_montserrat_30, DEFAULT_SELECTOR);

    // Create table for forecast data
    ui_2_table = lv_table_create(UI_Screens.Forecast_Screen);
    lv_obj_align(ui_2_table, LV_ALIGN_TOP_MID, 0, 40);
    //set font for table
    lv_obj_set_style_text_font(ui_2_table, &lv_font_unscii_16 , DEFAULT_SELECTOR);
    //set background color for table
    //lv_obj_set_style_bg_color(ui_2_table, lv_color_hex(0x000000), DEFAULT_SELECTOR);
    //set background color for table cells
    lv_obj_set_style_bg_color(ui_2_table, lv_color_hex(0x000000), LV_PART_ITEMS);
    lv_table_set_col_cnt(ui_2_table, 5);
    lv_table_set_row_cnt(ui_2_table, NUM_FORECAST_DAYS + 1); // +1 for header row
}

void ui_Forcast_screen_update(void)
{
    // Update slider value label
    if (ui_2_table != NULL)
    {
        updateTableWithForecastData(ui_2_table);
    }
}
