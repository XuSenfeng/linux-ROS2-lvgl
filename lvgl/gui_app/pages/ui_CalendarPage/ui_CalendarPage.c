#include "ui_CalendarPage.h"

///////////////////// VARIABLES ////////////////////

static const char *day_names[] =
{
    "日", "一", "二", "三", "四", "五", "六"
};

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

///////////////////// SCREEN init ////////////////////

void ui_CalendarPage_init(void *arg)
{
    lv_obj_t * ui_CalendarPage = lv_obj_create(NULL);
    // lv_obj_remove_flag(ui_CalendarPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_t * calendar = lv_calendar_create(ui_CalendarPage);
    lv_obj_set_width(calendar, UI_SCREEN_WIDTH);
    lv_obj_set_height(calendar, UI_SCREEN_WIDTH);
    lv_obj_align(calendar, LV_ALIGN_TOP_MID, 0, 0);
    int year; int month; int day; int hour; int minute; int second;
    sys_get_time(&year, &month, &day, &hour, &minute, &second);
    lv_calendar_set_today_date(calendar, year, month, day);
    lv_calendar_set_showed_date(calendar, year, month);

    lv_calendar_set_day_names(calendar, day_names);

    lv_calendar_set_chinese_mode(calendar, true);
    lv_obj_set_style_text_font(calendar, &ui_font_heiti14, LV_PART_MAIN);

    #if LV_USE_CALENDAR_HEADER_DROPDOWN
    lv_obj_t * calendar_head = lv_calendar_header_dropdown_create(calendar);
    #elif LV_USE_CALENDAR_HEADER_ARROW
        lv_calendar_header_arrow_create(calendar);
    #endif

    lv_obj_set_style_text_font(calendar_head, &lv_font_montserrat_14,  LV_PART_MAIN | LV_STATE_DEFAULT);

    // event
    lv_obj_add_event_cb(ui_CalendarPage, ui_enent_Gesture, LV_EVENT_ALL, NULL);

    // load page
    lv_scr_load_anim(ui_CalendarPage, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_CalendarPage_deinit()
{
    // deinit
}


void ui_calc_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_CalculatorBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_CalculatorBtn, 70);
    lv_obj_set_height(ui_CalculatorBtn, 70);
    lv_obj_set_x(ui_CalculatorBtn, x);
    lv_obj_set_y(ui_CalculatorBtn, y);
    lv_obj_add_flag(ui_CalculatorBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_CalculatorBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_CalculatorBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CalculatorBtn, lv_color_hex(0x707070), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CalculatorBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_CalculatorIcon = lv_label_create(ui_CalculatorBtn);
    lv_obj_set_width(ui_CalculatorIcon, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_CalculatorIcon, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_CalculatorIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_CalculatorIcon, "");
    lv_obj_set_style_text_color(ui_CalculatorIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_CalculatorIcon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_CalculatorIcon, &ui_font_iconfont48, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_CalculatorBtn, button_cb, LV_EVENT_CLICKED, "CalculatorPage");
}