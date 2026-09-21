#ifndef UI_PAGE_BASE_H
#define UI_PAGE_BASE_H

#include "lvgl.h"
#include "ui_manager.h"
#include <stdbool.h>

/*
 * Common base type for all UI pages.
 *
 * Usage:
 *   typedef struct {
 *       ui_page_t base;
 *       lv_obj_t *custom_widget;
 *   } settings_page_t;
 *
 *   static settings_page_t page;
 *
 *   page.base.root = ui_page_create_root(&page.base, color);
 *   ui_page_show(&page.base);
 *   ui_page_hide(&page.base);
 *   ui_page_destroy(&page.base);
 *
 * The base object owns the page root object and provides common
 * visibility, title, navigation, and destruction operations.
 */
typedef struct ui_page {
    lv_obj_t *root;
    const char *name;
    bool initialized;
} ui_page_t;

/* Create the root container for a page. */
lv_obj_t *ui_page_create_root(ui_page_t *page,
                              const char *name,
                              lv_color_t background);

/* Show or hide the page root. */
void ui_page_show(ui_page_t *page);
void ui_page_hide(ui_page_t *page);

/* Delete all LVGL objects owned by the page. */
void ui_page_destroy(ui_page_t *page);

/* Create common page controls. */
lv_obj_t *ui_page_create_title(ui_page_t *page, const char *title);

void ui_page_create_navigation(
    ui_page_t *page,
    const char *next_text,
    view_id_t next_id,
    bool show_back);

#endif