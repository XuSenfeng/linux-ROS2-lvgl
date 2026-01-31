#include "ui_ROSTest.h"
#include "rclcpp/rclcpp.hpp"

///////////////////// VARIABLES ////////////////////


///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////


static void ui_event_back_btn(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        lv_lib_pm_OpenPrePage(&page_manager);
    }
}



///////////////////// SCREEN init ////////////////////

void ui_ROSTest_init(void *arg)
{
    lv_obj_t * ROSTestPage = lv_obj_create(NULL);

    // 创建返回按钮
    lv_obj_t * back_btn = lv_btn_create(ROSTestPage);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 5, 10);
    lv_obj_set_size(back_btn, 75,40);
    lv_obj_t * back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label,"Back");
    lv_obj_align(back_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(back_btn_label, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(back_btn, ui_event_back_btn, LV_EVENT_ALL, NULL);


    // load page
    lv_scr_load_anim(ROSTestPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_ROSTest_deinit()
{
    // deinit
}


void ui_ROSTest_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_DrawBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_DrawBtn, 70);
    lv_obj_set_height(ui_DrawBtn, 70);
    lv_obj_set_x(ui_DrawBtn, x);
    lv_obj_set_y(ui_DrawBtn, y);
    lv_obj_add_flag(ui_DrawBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_DrawBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_DrawBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_DrawBtn, lv_color_hex(0x86A8E5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DrawBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_DrawBtn, &ui_img_paint60_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_DrawBtn, button_cb, LV_EVENT_CLICKED, (void *)"ROSTestPage");
}