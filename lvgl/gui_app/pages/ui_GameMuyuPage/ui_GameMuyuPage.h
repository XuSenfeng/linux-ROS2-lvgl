#ifndef _UI_GAMEMUYUPAGE_H
#define _UI_GAMEMUYUPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

void ui_GameMuyuPage_init(void *arg);
void ui_GameMuyuPage_deinit(void);
void ui_game_muyu_button(lv_obj_t * ui_HomeScreen, lv_obj_t * ui_AppIconContainer, int x, int y, void (*button_cb)(lv_event_t *));


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif