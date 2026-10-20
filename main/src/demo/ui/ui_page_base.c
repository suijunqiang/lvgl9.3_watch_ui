#include "ui_page_base.h"

static void page_next_event_cb(lv_event_t *event)
{
    lv_obj_t *button = lv_event_get_target(event);
    view_id_t target =
        (view_id_t)(uintptr_t)lv_obj_get_user_data(button);

    ui_push_view(target);
}

static void page_back_event_cb(lv_event_t *event)
{
    (void)event;
    ui_pop_view();
}

lv_obj_t *ui_page_create_root(ui_page_t *page,
                              const char *name,
                              lv_color_t background)
{
    if (page == NULL) {
        return NULL;
    }

    if (page->root != NULL) {
        return page->root;
    }

    page->name = name;
    page->root = lv_obj_create(lv_scr_act());

    lv_obj_set_size(page->root, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(page->root, background, 0);
    lv_obj_set_style_border_width(page->root, 0, 0);
    lv_obj_set_style_pad_all(page->root, 0, 0);

    page->initialized = true;
    return page->root;
}

void ui_page_show(ui_page_t *page)
{
    if (page != NULL && page->root != NULL) {
        lv_obj_clear_flag(page->root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_page_hide(ui_page_t *page)
{
    if (page != NULL && page->root != NULL) {
        lv_obj_add_flag(page->root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_page_destroy(ui_page_t *page)
{
    if (page == NULL) {
        return;
    }

    if (page->root != NULL) {
        lv_obj_del(page->root);
    }

    page->root = NULL;
    page->name = NULL;
    page->initialized = false;
}

/* --------------------------------------------------------------------- */
/* Default view lifecycle bound to a page                                 */
/* --------------------------------------------------------------------- */

void ui_page_view_init(ui_page_t *page, ui_page_build_t build)
{
    if (page == NULL || page->initialized) {
        return;
    }

    if (build != NULL) {
        build(page);
    }

    /* ui_page_create_root() sets the flag; enforce it for safety. */
    page->initialized = (page->root != NULL);
}

void ui_page_view_open(ui_page_t *page, ui_page_build_t build)
{
    ui_page_view_init(page, build);
    ui_page_show(page);
}

void ui_page_view_pause(ui_page_t *page)
{
    ui_page_hide(page);
}

void ui_page_view_resume(ui_page_t *page)
{
    ui_page_show(page);
}

void ui_page_view_close(ui_page_t *page)
{
    ui_page_hide(page);
}

void ui_page_view_deinit(ui_page_t *page)
{
    ui_page_destroy(page);
}

lv_obj_t *ui_page_create_title(ui_page_t *page, const char *title)
{
    if (page == NULL || page->root == NULL) {
        return NULL;
    }

    lv_obj_t *label = lv_label_create(page->root);
    lv_label_set_text(label, title);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 32);

    return label;
}

void ui_page_create_navigation(ui_page_t *page,
                               const char *next_text,
                               view_id_t next_id,
                               bool show_back)
{
    if (page == NULL || page->root == NULL) {
        return;
    }

    lv_obj_t *next_button = lv_btn_create(page->root);
    lv_obj_set_size(next_button, 118, 40);
    lv_obj_align(next_button, LV_ALIGN_BOTTOM_MID, 70, -18);
    lv_obj_set_user_data(next_button, (void *)(uintptr_t)next_id);
    lv_obj_add_event_cb(next_button,
                        page_next_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);

    lv_obj_t *next_label = lv_label_create(next_button);
    lv_label_set_text(next_label, next_text);
    lv_obj_center(next_label);

    if (!show_back) {
        return;
    }

    lv_obj_t *back_button = lv_btn_create(page->root);
    lv_obj_set_size(back_button, 118, 40);
    lv_obj_align(back_button, LV_ALIGN_BOTTOM_MID, -70, -18);
    lv_obj_add_event_cb(back_button,
                        page_back_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);

    lv_obj_t *back_label = lv_label_create(back_button);
    lv_label_set_text(back_label, "BACK");
    lv_obj_center(back_label);
}