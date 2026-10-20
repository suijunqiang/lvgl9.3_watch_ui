#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

static void setting_build(ui_page_t *page)
{
    ui_page_create_root(page, "SETTING", lv_color_hex(0x303350));
    ui_page_create_title(page, "SETTING");
    ui_page_create_navigation(page, "To Music", VIEW_MUSIC, true);
}

UI_VIEW_DEFINE(view_setting, &s_page, setting_build)
