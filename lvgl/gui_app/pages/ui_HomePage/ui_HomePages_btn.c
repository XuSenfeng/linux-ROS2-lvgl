#include "ui_HomePage.h"
#include <stdlib.h>
extern ui_desktop_data_t ui_desktop_data;



void temp_wvwnt_AppsBtn(lv_event_t * e)
{
    printf("Camera button clicked!\n");
    const char *cmd = "/usr/bin/gst-launch-1.0 filesrc location=/test.mp4 ! decodebin ! autovideosink";
    int ret = system(cmd);
    if(ret == -1)
    {
        LV_LOG_ERROR("Failed to execute command: %s", cmd);
    }
    else
    {
        LV_LOG_USER("Executed command: %s", cmd);
    }
    (void)ret;
}
















void ui_camera_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_CameraBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_CameraBtn, 70);
    lv_obj_set_height(ui_CameraBtn, 70);
    lv_obj_set_x(ui_CameraBtn, x);
    lv_obj_set_y(ui_CameraBtn, y);
    lv_obj_add_flag(ui_CameraBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_CameraBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_CameraBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CameraBtn, lv_color_hex(0xFF9A00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CameraBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_CameraIcon = lv_label_create(ui_CameraBtn);
    lv_obj_set_width(ui_CameraIcon, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_CameraIcon, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_CameraIcon, 2);
    lv_obj_set_y(ui_CameraIcon, 0);
    lv_obj_set_align(ui_CameraIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_CameraIcon, "");
    lv_obj_set_style_text_color(ui_CameraIcon, lv_color_hex(0xF6F7D7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_CameraIcon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_CameraIcon, &ui_font_iconfont48, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_CameraBtn, temp_wvwnt_AppsBtn, LV_EVENT_CLICKED, "CameraPage");
}



void ui_memo_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_MemoBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_MemoBtn, 70);
    lv_obj_set_height(ui_MemoBtn, 70);
    lv_obj_set_x(ui_MemoBtn, x);
    lv_obj_set_y(ui_MemoBtn, y);
    lv_obj_add_flag(ui_MemoBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_MemoBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_MemoBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MemoBtn, lv_color_hex(0xB83B5E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MemoBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_MemoBtn, &ui_img_memo64_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_MemoBtn, button_cb, LV_EVENT_CLICKED, "MemoPage");
}




