/*
 * Popup view: a page with a centered modal style card.
 */

#include "ui_views.h"
#include "ui_page_base.h"
#include "ui_theme.h"

static ui_page_t s_page;

static void popup_close_cb(lv_event_t *event)
{
    (void)event;
    ui_pop_view();
}

static void popup_build(ui_page_t *page)
{
    ui_page_create_root(page, "POPUP", UI_COLOR_BG);
    lv_obj_t *content = ui_page_create_header(page, "Popup", true);

    lv_obj_t *card = ui_card(content, UI_COLOR_SURFACE);
    lv_obj_set_width(card, LV_PCT(90));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(card, 20, LV_PART_MAIN);
    ui_style_flex(card, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 12);

    ui_label(card, LV_SYMBOL_BELL, UI_COLOR_ACCENT, UI_FONT_DISPLAY);
    ui_label(card, "New notification", UI_COLOR_TEXT, UI_FONT_TITLE);
    ui_label(card, "This is a popup view.", UI_COLOR_TEXT_DIM, UI_FONT_SMALL);

    lv_obj_t *btn = lv_button_create(card);
    lv_obj_set_size(btn, 140, 42);
    lv_obj_set_style_radius(btn, 14, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, UI_COLOR_ACCENT, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, UI_COLOR_ACCENT_2, LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(btn, popup_close_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = ui_label(btn, "Close", UI_COLOR_TEXT, UI_FONT_BODY);
    lv_obj_center(label);
}

UI_VIEW_DEFINE(view_popup, &s_page, popup_build)
