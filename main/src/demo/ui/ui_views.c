/*
 * Application side view registry.
 *
 * This file is the only place that knows which view owns which id, which
 * makes ui_manager.c a reusable, view agnostic engine. Adding a view means
 * adding one UI_VIEW_DESC() line here and one UI_VIEW_DECLARE() in ui_views.h.
 */

#include "ui_manager.h"
#include "ui_views.h"

/* Bind a view id to the lifecycle functions declared by UI_VIEW_DECLARE(). */
#define UI_VIEW_DESC(id_, prefix_)          \
    static const ui_view_t prefix_##_desc = \
    {                                       \
        .id = (id_),                        \
        .name = #id_,                       \
        .init = prefix_##_init,             \
        .open = prefix_##_open,             \
        .pause = prefix_##_pause,           \
        .resume = prefix_##_resume,         \
        .close = prefix_##_close,           \
        .deinit = prefix_##_deinit,         \
    }

UI_VIEW_DESC(VIEW_HOME,             view_home);
UI_VIEW_DESC(VIEW_SETTING,          view_setting);
UI_VIEW_DESC(VIEW_MUSIC,            view_music);
UI_VIEW_DESC(VIEW_WIFI,             view_wifi);
UI_VIEW_DESC(VIEW_POPUP,            view_popup);
UI_VIEW_DESC(VIEW_ANDROID_LAUNCHER, ui_android);
UI_VIEW_DESC(VIEW_WATERFALL,        ui_watch_waterfall);

static const ui_view_t *const g_views[] = {
    &view_home_desc,
    &view_setting_desc,
    &view_music_desc,
    &view_wifi_desc,
    &view_popup_desc,
    &ui_android_desc,
    &ui_watch_waterfall_desc,
};

void ui_views_register_all(void)
{
    for (unsigned i = 0; i < sizeof(g_views) / sizeof(g_views[0]); i++) {
        ui_manager_register(g_views[i]);
    }
}
