#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

static void music_build(ui_page_t *page)
{
    ui_page_create_root(page, "MUSIC", lv_color_hex(0x204030));
    ui_page_create_title(page, "MUSIC");
    ui_page_create_navigation(page, "To WiFi", VIEW_WIFI, true);
}

UI_VIEW_DEFINE(view_music, &s_page, music_build)
