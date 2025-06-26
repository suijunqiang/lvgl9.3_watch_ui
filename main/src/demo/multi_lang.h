#ifndef MULTI_LANG_H
#define MULTI_LANG_H

#include "../../../lvgl/lvgl.h"
#include <stdio.h>
#include <string.h>

/* 定义语言ID */
typedef enum {
    LANG_EN,
    LANG_CN,
    LANG_MAX
} language_t;

/* 当前语言 */
static language_t current_lang = LANG_EN;

/* 多语言文本表 */
static const char *lang_texts[LANG_MAX][20] = {
    /* 英文文本 */
    [LANG_EN] = {
        "LVGL Multilingual Demo",     /* TITLE */
        "Select Language:",           /* LANG_SELECT */
        "English",                    /* ENGLISH */
        "Chinese",                    /* CHINESE */
        "Welcome to LVGL demo!",      /* WELCOME */
        "This is a multilingual interface example.", /* DESC */
        "Button 1",                   /* BTN1 */
        "Button 2",                   /* BTN2 */
        "Settings",                   /* SETTINGS */
        "Version: 1.0.0"              /* VERSION */
    },
    /* 中文文本 */
    [LANG_CN] = {
        "LVGL 多语言演示",            /* TITLE */
        "选择语言:",                  /* LANG_SELECT */
        "英语",                       /* ENGLISH */
        "中文",                       /* CHINESE */
        "欢迎使用 LVGL 演示!",         /* WELCOME */
        "这是一个多语言界面示例。",     /* DESC */
        "按钮 1",                     /* BTN1 */
        "按钮 2",                     /* BTN2 */
        "设置",                       /* SETTINGS */
        "版本: 1.0.0"                 /* VERSION */
    }
};

/* 文本ID枚举，用于引用多语言文本表 */
typedef enum {
    TEXT_TITLE,
    TEXT_LANG_SELECT,
    TEXT_ENGLISH,
    TEXT_CHINESE,
    TEXT_WELCOME,
    TEXT_DESC,
    TEXT_BTN1,
    TEXT_BTN2,
    TEXT_SETTINGS,
    TEXT_VERSION,
    TEXT_MAX
} text_id_t;

/* UI 元素指针 */
static lv_obj_t *title_label;
static lv_obj_t *welcome_label;
static lv_obj_t *desc_label;
static lv_obj_t *btn1;
static lv_obj_t *btn2;
static lv_obj_t *lang_dropdown;
static lv_obj_t *version_label;

/* 获取当前语言的文本 */
const char *get_text(text_id_t text_id) {
    if (text_id < TEXT_MAX) {
        return lang_texts[current_lang][text_id];
    }
    return "Unknown text";
}

/* 更新所有UI元素的文本 */
static void update_ui_text(void) {
     lv_label_set_text(title_label, get_text(TEXT_TITLE));
    lv_label_set_text(welcome_label, get_text(TEXT_WELCOME));
    lv_label_set_text(desc_label, get_text(TEXT_DESC));
    lv_label_set_text(lv_obj_get_child(btn1, 0), get_text(TEXT_BTN1));
    lv_label_set_text(lv_obj_get_child(btn2, 0), get_text(TEXT_BTN2));
    
    /* 修正：正确拼接下拉菜单选项 */
    char options[100];
    snprintf(options, sizeof(options), "%s\n%s", 
             get_text(TEXT_ENGLISH), 
             get_text(TEXT_CHINESE));
    lv_dropdown_set_options(lang_dropdown, options);
    
    lv_label_set_text(version_label, get_text(TEXT_VERSION));
   }

/* 语言切换回调函数 */
static void lang_changed_event_cb(lv_event_t *e) {
    lv_obj_t *dropdown = lv_event_get_target(e);
    uint32_t sel = lv_dropdown_get_selected(dropdown);
    
    if (sel < LANG_MAX) {
        current_lang = (language_t)sel;
        update_ui_text();
    }
}
/* 创建多语言演示界面 */
void create_multilingual_demo(void) {
    /* 创建屏幕 */
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xf0f0f0), 0);
    
    /* 创建标题标签 */
    title_label = lv_label_create(screen);
    lv_label_set_text(title_label, get_text(TEXT_TITLE));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
    
    /* 创建语言选择下拉菜单 */
    lang_dropdown = lv_dropdown_create(screen);
    
    /* 修正：正确拼接下拉菜单选项 */
    char options[100];
    snprintf(options, sizeof(options), "%s\n%s", 
             get_text(TEXT_ENGLISH), 
             get_text(TEXT_CHINESE));
    lv_dropdown_set_options(lang_dropdown, options);
    
    lv_obj_add_event_cb(lang_dropdown, lang_changed_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(lang_dropdown, LV_ALIGN_TOP_RIGHT, -20, 20);
    
    /* 创建欢迎标签 */
    welcome_label = lv_label_create(screen);
    lv_label_set_text(welcome_label, get_text(TEXT_WELCOME));
    lv_obj_set_style_text_font(welcome_label, &lv_font_montserrat_18, 0);
    lv_obj_align_to(welcome_label, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    
    /* 创建描述标签 */
    desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, get_text(TEXT_DESC));
    lv_obj_set_style_text_font(desc_label, &lv_font_montserrat_14, 0);
    lv_obj_set_width(desc_label, 300);
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(desc_label, welcome_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    
    /* 创建按钮1 */
    btn1 = lv_btn_create(screen);
    lv_obj_t *btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, get_text(TEXT_BTN1));
    lv_obj_center(btn1_label);
    lv_obj_align_to(btn1, desc_label, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 30);
    
    /* 创建按钮2 */
    btn2 = lv_btn_create(screen);
    lv_obj_t *btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, get_text(TEXT_BTN2));
    lv_obj_center(btn2_label);
    lv_obj_align_to(btn2, desc_label, LV_ALIGN_OUT_BOTTOM_RIGHT, -20, 30);
    
    /* 创建版本标签 */
    version_label = lv_label_create(screen);
    lv_label_set_text(version_label, get_text(TEXT_VERSION));
    lv_obj_set_style_text_font(version_label, &lv_font_montserrat_12, 0);
    lv_obj_align(version_label, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/* 初始化中文支持 */
void init_chinese_font(void) {
    /* 确保LVGL配置中启用了中文字符支持 */
    /* 例如在lv_conf.h中设置:
     * #define LV_USE_FONT_MONTSERRAT_14 1
     * #define LV_FONT_MONTSERRAT_14_CHINESE 1
     */
}

#endif