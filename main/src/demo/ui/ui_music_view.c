#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_music_page = {0};

void view_music_init(void)
{
    if (g_music_page.initialized) {
        return;
    }

    ui_page_create_root(&g_music_page, "MUSIC", lv_color_hex(0x204030));
    ui_page_create_title(&g_music_page, "MUSIC");
    ui_page_create_navigation(&g_music_page, "To WiFi", VIEW_WIFI, true);
}

void view_music_open(void)
{
    if (!g_music_page.initialized) {
        view_music_init();
    }
    ui_page_show(&g_music_page);
}

void view_music_pause(void)
{
    ui_page_hide(&g_music_page);
}

void view_music_resume(void)
{
    ui_page_show(&g_music_page);
}

void view_music_close(void)
{
    ui_page_hide(&g_music_page);
}

void view_music_deinit(void)
{
    ui_page_destroy(&g_music_page);
}
