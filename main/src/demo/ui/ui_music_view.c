/*
 * Music view: a simple page with the shared header and a back button.
 */

#include "ui_views.h"
#include "ui_page_base.h"
#include "ui_theme.h"

static ui_page_t s_page;

static void music_build(ui_page_t *page)
{
    ui_page_create_root(page, "MUSIC", UI_COLOR_BG);
    lv_obj_t *content = ui_page_create_header(page, "Music", true);

    lv_obj_t *card = ui_card(content, UI_COLOR_ACCENT_2);
    lv_obj_set_width(card, LV_PCT(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(card, 20, LV_PART_MAIN);
    ui_style_flex(card, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 6);

    ui_label(card, LV_SYMBOL_AUDIO, UI_COLOR_TEXT, UI_FONT_DISPLAY);
    ui_label(card, "Now Playing", UI_COLOR_TEXT_DIM, UI_FONT_SMALL);
    ui_label(card, "No track", UI_COLOR_TEXT, UI_FONT_TITLE);
}

UI_VIEW_DEFINE(view_music, &s_page, music_build)
