#if 0
#include "ui_YOLOPage.h"
#if LV_USE_SIMULATOR == 0
#include "../../../../yolov5_demo/cpp/AIcamera_c_interface.h"
#endif

///////////////////// VARIABLES ////////////////////

#define DISP_WIDTH  320
#define DISP_HEIGHT 240
#define PIXEL_SIZE  2 // 对于RGB565格式来说是2字节每像素


lv_image_dsc_t img_dsc = {
    .header.w = 320,
    .header.h = 240,
    .data_size = 320 * 240 * 2,
    .header.cf = LV_COLOR_FORMAT_RGB565,
    .data = NULL,
};
lv_obj_t *img_cam; //要显示图像
static bool _first_into = true;

static lv_timer_t * timer;
///////////////////// ANIMATIONS ////////////////////


///////////////////// FUNCTIONS ////////////////////

static void ui_enent_Gesture(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_GESTURE)
    {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT || lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            lv_lib_pm_OpenPrePage(&page_manager);
        }
    }
}
#if LV_USE_SIMULATOR == 0
static void _ai_camera_init()
{
    const char * model_path = "./model/yolov5.rknn";
    start_ai_camera(model_path);
}

static void _ai_camera_deinit()
{
    stop_ai_camera();
}
#endif

static void timer_flash()
{
#if LV_USE_SIMULATOR == 0
    if(_first_into)
    {
        _first_into = false;
        // 分配内存给 img_dsc.data
        img_dsc.data = (uint8_t *)malloc(320 * 240 * 2);
        if (!img_dsc.data) {
            printf("Failed to allocate memory for img_dsc.data\n");
            // show msg box
            ui_msgbox_info("Error", "Failed to allocate memory for img_dsc.data");
            // 停止
            stop_ai_camera();
            // 返回上一页
            lv_lib_pm_OpenPrePage(&page_manager);
        }
        _ai_camera_init();
    }
    else
    {
        get_buf_data(img_dsc.data);
        lv_img_set_src(img_cam, &img_dsc);
    }
#endif
}

///////////////////// SCREEN init ////////////////////

void ui_YOLOPage_init(void *arg)
{

    _first_into = true;

    lv_obj_t * ui_YOLOPage = lv_obj_create(NULL);

    img_cam = lv_img_create(ui_YOLOPage);
    lv_obj_set_pos(img_cam, 0, 0);
    lv_obj_set_size(img_cam, DISP_WIDTH, DISP_HEIGHT);
    // 
    timer = lv_timer_create(timer_flash, 30, NULL);

    // event
    lv_obj_add_event_cb(ui_YOLOPage, ui_enent_Gesture, LV_EVENT_ALL, NULL);

    // load page
    lv_scr_load_anim(ui_YOLOPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);

}

/////////////////// SCREEN deinit ////////////////////

void ui_YOLOPage_deinit(void)
{
#if LV_USE_SIMULATOR == 0
    _ai_camera_deinit();
    // 释放 img_dsc.data 分配的内存
    free(img_dsc.data);
    img_dsc.data = NULL;
#endif
    lv_timer_del(timer);
}


void ui_yolo_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_YOLOBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_YOLOBtn, 70);
    lv_obj_set_height(ui_YOLOBtn, 70);
    lv_obj_set_x(ui_YOLOBtn, x);
    lv_obj_set_y(ui_YOLOBtn, y);
    lv_obj_add_flag(ui_YOLOBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_YOLOBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_YOLOBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_YOLOBtn, lv_color_hex(0xEEEF48), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_YOLOBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_YOLOLabel = lv_label_create(ui_YOLOBtn);
    lv_obj_set_width(ui_YOLOLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_YOLOLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_YOLOLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_YOLOLabel, "YOLO");
    lv_obj_set_style_text_color(ui_YOLOLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_YOLOLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_YOLOLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_YOLOBtn, button_cb, LV_EVENT_CLICKED, "YOLOPage");
}
#endif