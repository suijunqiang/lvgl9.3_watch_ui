#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_wifi_page = {0};

void view_wifi_init(void)
{
    if (g_wifi_page.initialized) {
        return;
    }

    ui_page_create_root(&g_wifi_page, "WIFI", lv_color_hex(0x101030));
    ui_page_create_title(&g_wifi_page, "WIFI");
    ui_page_create_navigation(&g_wifi_page, "To Popup", VIEW_POPUP, true);
}

void view_wifi_open(void)
{
    if (!g_wifi_page.initialized) {
        view_wifi_init();
    }
    ui_page_show(&g_wifi_page);
}

void view_wifi_pause(void)
{
    ui_page_hide(&g_wifi_page);
}

void view_wifi_resume(void)
{
    ui_page_show(&g_wifi_page);
}

void view_wifi_close(void)
{
    ui_page_hide(&g_wifi_page);
}

void view_wifi_deinit(void)
{
    ui_page_destroy(&g_wifi_page);
}
