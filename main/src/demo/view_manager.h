#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include "../../../lvgl/lvgl.h"

/* View ID */
typedef enum {
    VIEW_MAIN,
    VIEW_SETTINGS,
    VIEW_MUSIC,
    VIEW_GALLERY,
    VIEW_COUNT
} view_id_t;

/* View callback function types */
typedef void (*view_create_cb_t)(lv_obj_t * parent);
typedef void (*view_destroy_cb_t)(void);

/* Current active view */
extern view_id_t current_view;

/* View manager functions */
void view_manager_init(void);
void switch_to_view(view_id_t view_id);

#endif // MANAGER_VIEW_H