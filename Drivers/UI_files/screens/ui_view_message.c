// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: Buddy_Beacon

#include "../ui.h"

void ui_view_message_screen_init(void)
{
    ui_view_message = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_view_message, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label35 = lv_label_create(ui_view_message);
    lv_obj_set_width(ui_Label35, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label35, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label35, 0);
    lv_obj_set_y(ui_Label35, -21);
    lv_obj_set_align(ui_Label35, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label35, "^");

    ui_Label34 = lv_label_create(ui_view_message);
    lv_obj_set_width(ui_Label34, 118);
    lv_obj_set_height(ui_Label34, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label34, 0);
    lv_obj_set_y(ui_Label34, 4);
    lv_obj_set_align(ui_Label34, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label34, "Hate. Let me tell you how much I've come to hate you since I began to live.");
    lv_obj_set_style_text_color(ui_Label34, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label34, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

}
