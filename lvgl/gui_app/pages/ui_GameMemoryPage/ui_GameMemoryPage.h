#ifndef _UI_GAMEMEMORYPAGE_H
#define _UI_GAMEMEMORYPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

void ui_GameMemoryPage_init(void *arg);
void ui_GameMemoryPage_deinit(void);
void ui_game_memory_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *));


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif