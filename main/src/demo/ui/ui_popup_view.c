#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_popup_page = {0};

static void popup_back_cb(lv_event_t *event)
{
    (void)event;
    ui_pop_view();
}

void view_popup_init(void)
{
    if (g_popup_page.initialized) {
        return;
    }

    ui_page_create_root(&g_popup_page, "POPUP", lv_color_hex(0x404040));
    ui_page_create_title(&g_popup_page, "POPUP");

    lv_obj_t *popup = g_popup_page.root;
    lv_obj_set_size(popup, 320, 180);
    lv_obj_center(popup);

    lv_obj_t *btn_close = lv_btn_create(popup);
    lv_obj_set_size(btn_close, 110, 40);
    lv_obj_align(btn_close, LV_ALIGN_CENTER, 0, 30);
    lv_obj_add_event_cb(btn_close, popup_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lab_close = lv_label_create(btn_close);
    lv_label_set_text(lab_close, "CLOSE");
    lv_obj_center(lab_close);
}

void view_popup_open(void)
{
    if (!g_popup_page.initialized) {
        view_popup_init();
    }
    ui_page_show(&g_popup_page);
}

void view_popup_pause(void)
{
    ui_page_hide(&g_popup_page);
}

void view_popup_resume(void)
{
    ui_page_show(&g_popup_page);
}

void view_popup_close(void)
{
    ui_page_hide(&g_popup_page);
}

void view_popup_deinit(void)
{
    ui_page_destroy(&g_popup_page);
}
