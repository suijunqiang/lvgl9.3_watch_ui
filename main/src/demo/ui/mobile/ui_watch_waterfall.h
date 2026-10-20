#ifndef UI_WATCH_WATERFALL_H
#define UI_WATCH_WATERFALL_H

/*
 * Lifecycle of the watch waterfall view.
 *
 * The view is registered through ui_views.c / ui_views.h; this header only
 * exposes the six lifecycle functions to anyone that needs them directly.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void ui_watch_waterfall_init(void);
void ui_watch_waterfall_open(void);
void ui_watch_waterfall_pause(void);
void ui_watch_waterfall_resume(void);
void ui_watch_waterfall_close(void);
void ui_watch_waterfall_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_WATCH_WATERFALL_H */
