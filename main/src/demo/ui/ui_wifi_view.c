#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

static void wifi_build(ui_page_t *page)
{
    ui_page_create_root(page, "WIFI", lv_color_hex(0x101030));
    ui_page_create_title(page, "WIFI");
    ui_page_create_navigation(page, "To Popup", VIEW_POPUP, true);
}

UI_VIEW_DEFINE(view_wifi, &s_page, wifi_build)
