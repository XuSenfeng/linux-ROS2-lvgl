#include "ui_TimerPage.h"

///////////////////// VARIABLES ////////////////////

static lv_obj_t *s_timer_page = NULL;
static lv_obj_t *s_time_label = NULL;
static lv_obj_t *s_pomodoro_label = NULL;
static lv_obj_t *s_pomodoro_btn = NULL;
static lv_obj_t *btn_label = NULL;
static lv_timer_t *s_timer = NULL;
static lv_display_rotation_t s_prev_rotation = LV_DISPLAY_ROTATION_0;
static bool s_rotation_changed = false;

typedef enum {
    POMODORO_STATE_IDLE = 0,
    POMODORO_STATE_WORK,
    POMODORO_STATE_SHORT_BREAK,
    POMODORO_STATE_LONG_BREAK
} pomodoro_state_t;

static pomodoro_state_t s_pomodoro_state = POMODORO_STATE_IDLE;
static int32_t s_pomodoro_remaining_sec = 25 * 60;
static uint8_t s_pomodoro_rounds = 0;

LV_IMG_DECLARE(tomato80x69); // Declare the image resource
///////////////////// ANIMATIONS ////////////////////
static void ui_event_gesture(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_GESTURE) 
    {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT || lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            lv_lib_pm_OpenPrePage(&page_manager);
        }
    }
}

///////////////////// FUNCTIONS ////////////////////

static void ui_timer_update_cb(lv_timer_t *timer)
{
    LV_UNUSED(timer);
    if (!s_time_label) {
        return;
    }

    uint32_t year, month, day, sec, min, hour;
    sys_get_time((int *)&year, (int *)&month, (int *)&day, (int *)&hour, (int *)&min, (int *)&sec);

    if (s_pomodoro_label) {
        if (s_pomodoro_state != POMODORO_STATE_IDLE) {
            if (s_pomodoro_remaining_sec > 0) {
                s_pomodoro_remaining_sec--;
            } else {
                if (s_pomodoro_state == POMODORO_STATE_WORK) {
                    s_pomodoro_rounds++;
                    if ((s_pomodoro_rounds % 4U) == 0U) {
                        s_pomodoro_state = POMODORO_STATE_LONG_BREAK;
                        s_pomodoro_remaining_sec = 15 * 60;
                    } else {
                        s_pomodoro_state = POMODORO_STATE_SHORT_BREAK;
                        s_pomodoro_remaining_sec = 5 * 60;
                    }
                } else {
                    s_pomodoro_state = POMODORO_STATE_WORK;
                    s_pomodoro_remaining_sec = 25 * 60;
                }
            }
        }

        const char *state_text = "番茄";
        switch (s_pomodoro_state) {
            case POMODORO_STATE_WORK:
                state_text = "工作";
                break;
            case POMODORO_STATE_SHORT_BREAK:
                state_text = "短休";
                break;
            case POMODORO_STATE_LONG_BREAK:
                state_text = "长休";
                break;
            default:
                state_text = "番茄";
                break;
        }

        int32_t remain = s_pomodoro_remaining_sec;
        if (remain < 0) remain = 0;
        int32_t mm = remain / 60;
        int32_t ss = remain % 60;
        if(s_pomodoro_state == POMODORO_STATE_IDLE){
            lv_label_set_text_fmt(s_pomodoro_label, "%s %02d:%02d", state_text, (int)mm, (int)ss);
            lv_label_set_text_fmt(s_time_label, "%02d:%02d:%02d", (int)hour, (int)min, (int)sec);
        }else{
            lv_label_set_text_fmt(s_pomodoro_label, "%s %02d:%02d:%02d", state_text, (int)hour, (int)min, (int)sec);
            lv_label_set_text_fmt(s_time_label, "%02d:%02d", (int)mm, (int)ss);
        }

    }
}


///////////////////// SCREEN init ////////////////////

static void ui_pomodoro_btn_cb(lv_event_t * e)
{
    LV_UNUSED(e);
    if(s_pomodoro_state != POMODORO_STATE_IDLE) {
        // stop pomodoro
        s_pomodoro_state = POMODORO_STATE_IDLE;
        s_pomodoro_remaining_sec = 25 * 60;
        s_pomodoro_rounds = 0;
        lv_obj_set_style_text_font(s_time_label, &font_led90, 0);
        lv_label_set_text(btn_label, "开始");
    } else {
        // start pomodoro
        s_pomodoro_state = POMODORO_STATE_WORK;
        s_pomodoro_remaining_sec = 25 * 60;
        lv_obj_set_style_text_font(s_time_label, &font_led128, 0);
        lv_label_set_text(btn_label, "结束");
    }
    ui_timer_update_cb(NULL);

}

void ui_TimerPage_init(void *arg)
{
    // LV_UNUSED(arg);
    printf("timer ...");
    s_timer_page = lv_obj_create(NULL);
    lv_obj_clear_flag(s_timer_page, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(s_timer_page, lv_color_hex(0xFDECEC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(s_timer_page, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    int32_t screen_w = lv_display_get_horizontal_resolution(lv_display_get_default());
    int32_t screen_h = lv_display_get_vertical_resolution(lv_display_get_default());

    lv_obj_t *card = lv_obj_create(s_timer_page);
    lv_obj_set_size(card, screen_w - 40, screen_h - 40);
    lv_obj_center(card);
    lv_obj_set_style_radius(card, 24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(card, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(card, lv_color_hex(0xD66B6B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(card, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(card, 24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *tomato = lv_image_create(card);
    lv_image_set_src(tomato, &tomato80x69);
    lv_obj_align(tomato, LV_ALIGN_TOP_LEFT, 20, 20);

    lv_obj_t *title = lv_label_create(card);
    lv_label_set_text(title, "番茄时钟");
    lv_obj_set_style_text_font(title, &tomato_font32, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xC0392B), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 120, 30);

    // lv_display_t *disp = lv_display_get_default();
    // if (disp) {
    //     s_prev_rotation = lv_display_get_rotation(disp);
    //     lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);
    //     s_rotation_changed = true;
    // } else {
    //     s_rotation_changed = false;
    // }

    s_time_label = lv_label_create(card);
    lv_obj_set_style_text_font(s_time_label, &font_led90, 0);
    lv_label_set_text(s_time_label, "00:00:00");
    lv_obj_set_style_text_color(s_time_label, lv_color_hex(0x2C3E50), 0);
    lv_obj_align(s_time_label, LV_ALIGN_CENTER, 0, -10);

    s_pomodoro_label = lv_label_create(card);
    lv_obj_set_style_text_font(s_pomodoro_label, &tomato_font32, 0);
    lv_label_set_text(s_pomodoro_label, "番茄 25:00");
    lv_obj_set_style_text_color(s_pomodoro_label, lv_color_hex(0xC0392B), 0);
    lv_obj_align(s_pomodoro_label, LV_ALIGN_CENTER, 0, 90);

    s_pomodoro_btn = lv_button_create(card);
    lv_obj_set_size(s_pomodoro_btn, 160, 56);
    lv_obj_set_style_radius(s_pomodoro_btn, 28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(s_pomodoro_btn, lv_color_hex(0xE74C3C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(s_pomodoro_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_pomodoro_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(s_pomodoro_btn, ui_pomodoro_btn_cb, LV_EVENT_CLICKED, NULL);

    btn_label = lv_label_create(s_pomodoro_btn);
    lv_label_set_text(btn_label, "开始");
    lv_obj_set_style_text_font(btn_label, &tomato_font32, 0);
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(btn_label);

    if (s_timer) {
        lv_timer_del(s_timer);
        s_timer = NULL;
    }
    s_timer = lv_timer_create(ui_timer_update_cb, 1000, NULL);
    ui_timer_update_cb(NULL);

    lv_obj_add_event_cb(s_timer_page, ui_event_gesture, LV_EVENT_GESTURE, NULL);
    // load page
    lv_scr_load_anim(s_timer_page, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/////////////////// SCREEN deinit ////////////////////

void ui_TimerPage_deinit()
{
    if (s_timer) {
        lv_timer_del(s_timer);
        s_timer = NULL;
    }
    // if (s_rotation_changed) {
    //     lv_display_t *disp = lv_display_get_default();
    //     if (disp) {
    //         lv_display_set_rotation(disp, s_prev_rotation);
    //     }
    //     s_rotation_changed = false;
    // }
}


void ui_timer_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *)){
    lv_obj_t * ui_TimerBtn = lv_button_create(ui_AppIconContainer);
    lv_obj_set_width(ui_TimerBtn, 70);
    lv_obj_set_height(ui_TimerBtn, 70);
    lv_obj_set_x(ui_TimerBtn, x);
    lv_obj_set_y(ui_TimerBtn, y);
    lv_obj_add_flag(ui_TimerBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_remove_flag(ui_TimerBtn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_TimerBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_TimerBtn, lv_color_hex(0x11999E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TimerBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Icon2048 = lv_label_create(ui_TimerBtn);
    lv_obj_set_width(ui_Icon2048, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Icon2048, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_Icon2048, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Icon2048, "");
    lv_obj_set_style_text_color(ui_Icon2048, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Icon2048, 196, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Icon2048, &ui_font_iconfont20, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_add_event_cb(ui_TimerBtn, button_cb, LV_EVENT_CLICKED, "TimerPage");
}