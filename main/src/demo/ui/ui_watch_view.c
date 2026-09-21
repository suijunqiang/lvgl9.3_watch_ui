#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_watch_page = {0};

void ui_watchwaterfall_init(void)
{
    if (g_watch_page.initialized) {
        return;
    }

    ui_page_create_root(&g_watch_page, "WATCH", lv_color_hex(0x2A2A2A));
    ui_page_create_title(&g_watch_page, "WATCH");
    ui_page_create_navigation(&g_watch_page, "Back to Home", VIEW_HOME, true);
}

void ui_watchwaterfall_open(void)
{
    if (!g_watch_page.initialized) {
        ui_watchwaterfall_init();
    }
    ui_page_show(&g_watch_page);
}

void ui_watchwaterfall_pause(void)
{
    ui_page_hide(&g_watch_page);
}

void ui_watchwaterfall_resume(void)
{
    ui_page_show(&g_watch_page);
}

void ui_watchwaterfall_close(void)
{
    ui_page_hide(&g_watch_page);
}

void ui_watchwaterfall_deinit(void)
{
    ui_page_destroy(&g_watch_page);
}
