#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_setting_page = {0};

void view_setting_init(void)
{
    if (g_setting_page.initialized) {
        return;
    }

    ui_page_create_root(&g_setting_page, "SETTING", lv_color_hex(0x303350));
    ui_page_create_title(&g_setting_page, "SETTING");
    ui_page_create_navigation(&g_setting_page, "To Music", VIEW_MUSIC, true);
}

void view_setting_open(void)
{
    if (!g_setting_page.initialized) {
        view_setting_init();
    }
    ui_page_show(&g_setting_page);
}

void view_setting_pause(void)
{
    ui_page_hide(&g_setting_page);
}

void view_setting_resume(void)
{
    ui_page_show(&g_setting_page);
}

void view_setting_close(void)
{
    ui_page_hide(&g_setting_page);
}

void view_setting_deinit(void)
{
    ui_page_destroy(&g_setting_page);
}
