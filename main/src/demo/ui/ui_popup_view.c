#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

static void popup_back_cb(lv_event_t *event)
{
    (void)event;
    ui_pop_view();
}

static void popup_build(ui_page_t *page)
{
    ui_page_create_root(page, "POPUP", lv_color_hex(0x404040));
    ui_page_create_title(page, "POPUP");

    lv_obj_t *popup = page->root;
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

UI_VIEW_DEFINE(view_popup, &s_page, popup_build)
