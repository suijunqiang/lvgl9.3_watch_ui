#ifndef UI_ANDROID_H
#define UI_ANDROID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// Android Launcher view lifecycle
void ui_android_init(void);
void ui_android_open(void);
void ui_android_pause(void);
void ui_android_resume(void);
void ui_android_close(void);
void ui_android_deinit(void);

#ifdef __cplusplus
}
#endif

#endif