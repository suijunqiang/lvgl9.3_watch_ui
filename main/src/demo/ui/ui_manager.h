#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "ui_views.h"

void ui_manager_init(void);
void ui_push_view(view_id_t to_id);
void ui_pop_view(void);
void ui_switch_view(view_id_t id);
view_id_t ui_current_view(void);

#endif