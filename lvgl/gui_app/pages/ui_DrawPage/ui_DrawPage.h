#ifndef _UI_DRAWPAGE_H
#define _UI_DRAWPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

void ui_DrawPage_init(void *arg);
void ui_DrawPage_deinit(void);
void ui_draw_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *));

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif