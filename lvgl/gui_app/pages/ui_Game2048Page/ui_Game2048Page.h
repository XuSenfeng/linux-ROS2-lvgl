#ifndef _UI_GAME2048PAGE_H
#define _UI_GAME2048PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

void ui_Game2048Page_init(void *arg);
void ui_Game2048Page_deinit(void);
void ui_game_2048_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *));

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif