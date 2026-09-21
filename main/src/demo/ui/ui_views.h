#ifndef UI_VIEWS_H
#define UI_VIEWS_H

#include "lvgl.h"
#include <stdbool.h>

typedef enum {
    VIEW_NONE = -1,
    VIEW_HOME = 0,
    VIEW_SETTING,
    VIEW_MUSIC,
    VIEW_WIFI,
    VIEW_POPUP,
    VIEW_ANDROID_LAUNCHER,
    VIEW_WATERFALL,
    VIEW_MAX
} view_id_t;

typedef struct {
    view_id_t id;
    bool created;
    void (*init)(void);
    void (*open)(void);
    void (*pause)(void);
    void (*resume)(void);
    void (*close)(void);
    void (*deinit)(void);
} ui_view_t;

void view_home_init(void);
void view_home_open(void);
void view_home_pause(void);
void view_home_resume(void);
void view_home_close(void);
void view_home_deinit(void);

void view_setting_init(void);
void view_setting_open(void);
void view_setting_pause(void);
void view_setting_resume(void);
void view_setting_close(void);
void view_setting_deinit(void);

void view_music_init(void);
void view_music_open(void);
void view_music_pause(void);
void view_music_resume(void);
void view_music_close(void);
void view_music_deinit(void);

void view_wifi_init(void);
void view_wifi_open(void);
void view_wifi_pause(void);
void view_wifi_resume(void);
void view_wifi_close(void);
void view_wifi_deinit(void);

void view_popup_init(void);
void view_popup_open(void);
void view_popup_pause(void);
void view_popup_resume(void);
void view_popup_close(void);
void view_popup_deinit(void);

void ui_android_init(void);
void ui_android_open(void);
void ui_android_pause(void);
void ui_android_resume(void);
void ui_android_close(void);
void ui_android_deinit(void);

void ui_watchwaterfall_init(void);
void ui_watchwaterfall_open(void);
void ui_watchwaterfall_pause(void);
void ui_watchwaterfall_resume(void);
void ui_watchwaterfall_close(void);
void ui_watchwaterfall_deinit(void);
#endif