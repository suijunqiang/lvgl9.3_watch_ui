/*
 * Settings view: a simple page with the shared header and a back button.
 */

#include "ui_views.h"
#include "ui_page_base.h"
#include "ui_theme.h"

static ui_page_t s_page;

static void setting_build(ui_page_t *page)
{
    ui_page_create_root(page, "SETTING", UI_COLOR_BG);
    lv_obj_t *content = ui_page_create_header(page, "Settings", true);

    lv_obj_t *card = ui_card(content, UI_COLOR_SURFACE);
    lv_obj_set_width(card, LV_PCT(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(card, 16, LV_PART_MAIN);
    ui_style_flex(card, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, 10);

    ui_label(card, LV_SYMBOL_SETTINGS, UI_COLOR_TEXT_DIM, UI_FONT_BODY);
    ui_label(card, "Settings", UI_COLOR_TEXT, UI_FONT_TITLE);
    ui_label(card, "Configure your watch preferences.", UI_COLOR_TEXT_DIM,
             UI_FONT_SMALL);
}

UI_VIEW_DEFINE(view_setting, &s_page, setting_build)
