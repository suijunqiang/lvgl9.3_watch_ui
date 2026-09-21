#ifndef UI_WATCH_WATERFALL_H
#define UI_WATCH_WATERFALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/* Watch waterfall view lifecycle */
void ui_watch_waterfall_init(void);
void ui_watch_waterfall_open(void);
void ui_watch_waterfall_pause(void);
void ui_watch_waterfall_resume(void);
void ui_watch_waterfall_close(void);
void ui_watch_waterfall_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // UI_WATCH_WATERFALL_H
