#ifndef UI_PAGE_BASE_H
#define UI_PAGE_BASE_H

/*
 * Layer 3 - shared page container.
 *
 * A page is a full screen container owned by a view. It provides the
 * common visibility, title, navigation and destruction operations, plus
 * a default implementation of the six view lifecycle functions.
 *
 * Usage inside a view file:
 *
 *   static ui_page_t s_page;
 *
 *   static void build(ui_page_t *page)
 *   {
 *       ui_page_create_root(page, "SETTING", UI_COLOR_BG);
 *       ui_page_create_header(page, "Settings", true);  // true = show back
 *       ...
 *   }
 *
 *   UI_VIEW_DEFINE(view_setting, &s_page, build)
 */

#include "lvgl.h"
#include "ui_manager.h"
#include "ui_theme.h"
#include <stdbool.h>

typedef struct ui_page {
    lv_obj_t *root;      /* full screen container                     */
    lv_obj_t *content;   /* area below the header, for page widgets   */
    const char *name;
    bool initialized;
} ui_page_t;

/* Create the root container for a page. */
lv_obj_t *ui_page_create_root(ui_page_t *page,
                              const char *name,
                              lv_color_t background);

/*
 * Create the standard header: title on the left, optional back arrow on
 * the right. Page content should be added to page->content, which is
 * created by this call and sized to the area below the header.
 *
 * A back button is only useful on a pushed (non root) view; on the root
 * view pass show_back = false.
 */
lv_obj_t *ui_page_create_header(ui_page_t *page,
                                const char *title,
                                bool show_back);

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

/* --------------------------------------------------------------------- */
/* Default view lifecycle bound to a page                                 */
/* --------------------------------------------------------------------- */

/* Builds the widgets of a page, called at most once per view instance. */
typedef void (*ui_page_build_t)(ui_page_t *page);

/*
 * Generic lifecycle, safe to call at any time:
 *   init  -> run `build` once (page must call ui_page_create_root())
 *   open  -> init when needed, then show
 *   pause / close -> hide
 *   resume         -> show
 *   deinit         -> delete every widget of the page
 */
void ui_page_view_init(ui_page_t *page, ui_page_build_t build);
void ui_page_view_open(ui_page_t *page, ui_page_build_t build);
void ui_page_view_pause(ui_page_t *page);
void ui_page_view_resume(ui_page_t *page);
void ui_page_view_close(ui_page_t *page);
void ui_page_view_deinit(ui_page_t *page);

/*
 * Implement the six functions declared by UI_VIEW_DECLARE(prefix_).
 * No trailing semicolon needed.
 *
 *   UI_VIEW_DEFINE(view_home, &s_page, build)
 */
#define UI_VIEW_DEFINE(prefix_, page_, build_)                              \
    void prefix_##_init(void)   { ui_page_view_init((page_), (build_)); }   \
    void prefix_##_open(void)   { ui_page_view_open((page_), (build_)); }   \
    void prefix_##_pause(void)  { ui_page_view_pause((page_)); }            \
    void prefix_##_resume(void) { ui_page_view_resume((page_)); }           \
    void prefix_##_close(void)  { ui_page_view_close((page_)); }            \
    void prefix_##_deinit(void) { ui_page_view_deinit((page_)); }

#endif
