/*
 * WiFi view: a simple page with the shared header and a back button.
 */

#include "ui_views.h"
#include "ui_page_base.h"
#include "ui_theme.h"

static ui_page_t s_page;

static void wifi_build(ui_page_t *page)
{
    ui_page_create_root(page, "WIFI", UI_COLOR_BG);
    lv_obj_t *content = ui_page_create_header(page, "WiFi", true);

    lv_obj_t *card = ui_card(content, UI_COLOR_SURFACE);
    lv_obj_set_width(card, LV_PCT(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(card, 16, LV_PART_MAIN);
    ui_style_flex(card, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, 0);

    lv_obj_t *left = lv_obj_create(card);
    lv_obj_set_size(left, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(left);
    ui_style_flex(left, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, 2);

    ui_label(left, LV_SYMBOL_WIFI, UI_COLOR_ACCENT, UI_FONT_ICON);
    ui_label(left, "Connected", UI_COLOR_TEXT, UI_FONT_BODY);

    ui_label(card, "90%", UI_COLOR_TEXT_DIM, UI_FONT_SMALL);
}

UI_VIEW_DEFINE(view_wifi, &s_page, wifi_build)
