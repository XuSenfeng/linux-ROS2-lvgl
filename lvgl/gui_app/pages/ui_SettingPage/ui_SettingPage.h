#ifndef _UI_SETTINGPAGE_H
#define _UI_SETTINGPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

void ui_SettingPage_init(void *arg);
void ui_SettingPage_deinit(void);
void ui_setting_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *));

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif