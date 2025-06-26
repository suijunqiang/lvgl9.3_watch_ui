#ifndef MULTI_LANG_H
#define MULTI_LANG_H

#include "../../../lvgl/lvgl.h"
#include <stdio.h>
#include <string.h>

/* ��������ID */
typedef enum {
    LANG_EN,
    LANG_CN,
    LANG_MAX
} language_t;

/* ��ǰ���� */
static language_t current_lang = LANG_EN;

/* �������ı��� */
static const char *lang_texts[LANG_MAX][20] = {
    /* Ӣ���ı� */
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
    /* �����ı� */
    [LANG_CN] = {
        "LVGL ��������ʾ",            /* TITLE */
        "ѡ������:",                  /* LANG_SELECT */
        "Ӣ��",                       /* ENGLISH */
        "����",                       /* CHINESE */
        "��ӭʹ�� LVGL ��ʾ!",         /* WELCOME */
        "����һ�������Խ���ʾ����",     /* DESC */
        "��ť 1",                     /* BTN1 */
        "��ť 2",                     /* BTN2 */
        "����",                       /* SETTINGS */
        "�汾: 1.0.0"                 /* VERSION */
    }
};

/* �ı�IDö�٣��������ö������ı��� */
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

/* UI Ԫ��ָ�� */
static lv_obj_t *title_label;
static lv_obj_t *welcome_label;
static lv_obj_t *desc_label;
static lv_obj_t *btn1;
static lv_obj_t *btn2;
static lv_obj_t *lang_dropdown;
static lv_obj_t *version_label;

/* ��ȡ��ǰ���Ե��ı� */
const char *get_text(text_id_t text_id) {
    if (text_id < TEXT_MAX) {
        return lang_texts[current_lang][text_id];
    }
    return "Unknown text";
}

/* ��������UIԪ�ص��ı� */
static void update_ui_text(void) {
     lv_label_set_text(title_label, get_text(TEXT_TITLE));
    lv_label_set_text(welcome_label, get_text(TEXT_WELCOME));
    lv_label_set_text(desc_label, get_text(TEXT_DESC));
    lv_label_set_text(lv_obj_get_child(btn1, 0), get_text(TEXT_BTN1));
    lv_label_set_text(lv_obj_get_child(btn2, 0), get_text(TEXT_BTN2));
    
    /* ��������ȷƴ�������˵�ѡ�� */
    char options[100];
    snprintf(options, sizeof(options), "%s\n%s", 
             get_text(TEXT_ENGLISH), 
             get_text(TEXT_CHINESE));
    lv_dropdown_set_options(lang_dropdown, options);
    
    lv_label_set_text(version_label, get_text(TEXT_VERSION));
   }

/* �����л��ص����� */
static void lang_changed_event_cb(lv_event_t *e) {
    lv_obj_t *dropdown = lv_event_get_target(e);
    uint32_t sel = lv_dropdown_get_selected(dropdown);
    
    if (sel < LANG_MAX) {
        current_lang = (language_t)sel;
        update_ui_text();
    }
}
/* ������������ʾ���� */
void create_multilingual_demo(void) {
    /* ������Ļ */
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xf0f0f0), 0);
    
    /* ���������ǩ */
    title_label = lv_label_create(screen);
    lv_label_set_text(title_label, get_text(TEXT_TITLE));
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
    
    /* ��������ѡ�������˵� */
    lang_dropdown = lv_dropdown_create(screen);
    
    /* ��������ȷƴ�������˵�ѡ�� */
    char options[100];
    snprintf(options, sizeof(options), "%s\n%s", 
             get_text(TEXT_ENGLISH), 
             get_text(TEXT_CHINESE));
    lv_dropdown_set_options(lang_dropdown, options);
    
    lv_obj_add_event_cb(lang_dropdown, lang_changed_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(lang_dropdown, LV_ALIGN_TOP_RIGHT, -20, 20);
    
    /* ������ӭ��ǩ */
    welcome_label = lv_label_create(screen);
    lv_label_set_text(welcome_label, get_text(TEXT_WELCOME));
    lv_obj_set_style_text_font(welcome_label, &lv_font_montserrat_18, 0);
    lv_obj_align_to(welcome_label, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    
    /* ����������ǩ */
    desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, get_text(TEXT_DESC));
    lv_obj_set_style_text_font(desc_label, &lv_font_montserrat_14, 0);
    lv_obj_set_width(desc_label, 300);
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_align_to(desc_label, welcome_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    
    /* ������ť1 */
    btn1 = lv_btn_create(screen);
    lv_obj_t *btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, get_text(TEXT_BTN1));
    lv_obj_center(btn1_label);
    lv_obj_align_to(btn1, desc_label, LV_ALIGN_OUT_BOTTOM_LEFT, 20, 30);
    
    /* ������ť2 */
    btn2 = lv_btn_create(screen);
    lv_obj_t *btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, get_text(TEXT_BTN2));
    lv_obj_center(btn2_label);
    lv_obj_align_to(btn2, desc_label, LV_ALIGN_OUT_BOTTOM_RIGHT, -20, 30);
    
    /* �����汾��ǩ */
    version_label = lv_label_create(screen);
    lv_label_set_text(version_label, get_text(TEXT_VERSION));
    lv_obj_set_style_text_font(version_label, &lv_font_montserrat_12, 0);
    lv_obj_align(version_label, LV_ALIGN_BOTTOM_MID, 0, -20);
}

/* ��ʼ������֧�� */
void init_chinese_font(void) {
    /* ȷ��LVGL�����������������ַ�֧�� */
    /* ������lv_conf.h������:
     * #define LV_USE_FONT_MONTSERRAT_14 1
     * #define LV_FONT_MONTSERRAT_14_CHINESE 1
     */
}

#endif