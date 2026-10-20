/*
 * Android launcher view (VIEW_ANDROID_LAUNCHER).
 *
 * A phone style launcher: status bar, three swipeable pages of app icons,
 * a page indicator and the shared back button.
 *
 * This view manages its own widgets (no ui_page_t) because it needs a
 * tileview, but it follows the same theme and navigation rules as the
 * other views: the back button pops the stack.
 */

#include "../ui_views.h"
#include "../ui_page_base.h"
#include "../ui_theme.h"
#include <stdio.h>

#include "ui_android.h"

#define STATUS_BAR_H   34
#define NAV_BAR_H      56
#define INDICATOR_H    20
#define PAGE_COUNT     3
#define APPS_PER_PAGE  10
#define APP_ICON_SIZE  58
#define APP_ICON_GAP   10

static lv_obj_t *s_root;
static lv_obj_t *s_tileview;
static lv_obj_t *s_indicators[PAGE_COUNT];
static lv_obj_t *s_tiles[PAGE_COUNT];



static void android_back_cb(lv_event_t *event)
{
    (void)event;
    ui_pop_view();
}

static void android_status_bar_create(lv_obj_t *parent)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_set_size(bar, LV_PCT(100), STATUS_BAR_H);
    ui_style_surface(bar, UI_COLOR_SURFACE);
    ui_style_flex(bar, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_hor(bar, UI_PAD, LV_PART_MAIN);

    ui_label(bar, "10:24", UI_COLOR_TEXT, UI_FONT_BODY);

    lv_obj_t *right = lv_obj_create(bar);
    lv_obj_set_size(right, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(right);
    ui_style_flex(right, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 12);

    ui_label(right, LV_SYMBOL_WIFI, UI_COLOR_TEXT, UI_FONT_SMALL);
    ui_label(right, LV_SYMBOL_BLUETOOTH, UI_COLOR_TEXT, UI_FONT_SMALL);
    ui_label(right, "85%", UI_COLOR_TEXT_DIM, UI_FONT_SMALL);
}

/*
 * One launcher page: a flex-wrapped grid of app icons, one distinct
 * colour per app, with the app index printed in the middle.
 */
static void android_page_create(lv_obj_t *tile, int page_idx)
{
    static const lv_color_t colors[APPS_PER_PAGE] = {
        LV_COLOR_MAKE(0xFF, 0x52, 0x52), LV_COLOR_MAKE(0x4C, 0xAF, 0x50),
        LV_COLOR_MAKE(0x21, 0x96, 0xF3), LV_COLOR_MAKE(0xFF, 0xC1, 0x07),
        LV_COLOR_MAKE(0x9C, 0x27, 0xB0), LV_COLOR_MAKE(0x00, 0xBC, 0xD4),
        LV_COLOR_MAKE(0xFF, 0x98, 0x00), LV_COLOR_MAKE(0x8B, 0xC3, 0x4A),
        LV_COLOR_MAKE(0xE9, 0x1E, 0x63), LV_COLOR_MAKE(0x60, 0x7D, 0x8B),
    };

    ui_style_surface(tile, UI_COLOR_BG);
    lv_obj_set_style_pad_all(tile, 12, LV_PART_MAIN);
    lv_obj_set_flex_flow(tile, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(tile,
                          LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(tile, APP_ICON_GAP, LV_PART_MAIN);

    for (int i = 0; i < APPS_PER_PAGE; i++) {
        lv_obj_t *icon = lv_button_create(tile);
        lv_obj_set_size(icon, APP_ICON_SIZE, APP_ICON_SIZE);
        lv_obj_set_style_radius(icon, 16, LV_PART_MAIN);
        lv_obj_set_style_bg_color(icon, colors[i], LV_PART_MAIN);
        lv_obj_set_style_shadow_width(icon, 0, LV_PART_MAIN);

        char label[16];
        snprintf(label, sizeof(label), "%d-%d", page_idx, i + 1);
        lv_obj_t *text = ui_label(icon, label, UI_COLOR_TEXT, UI_FONT_SMALL);
        lv_obj_center(text);
    }
}

static void android_tile_change_cb(lv_event_t *event)
{
    (void)event;

    lv_obj_t *active = lv_tileview_get_tile_act(s_tileview);

    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_set_style_bg_color(s_indicators[i],
                                  (active == s_tiles[i]) ? UI_COLOR_ACCENT
                                                         : UI_COLOR_TILE,
                                  LV_PART_MAIN);
    }
}

/* Tapping a page dot jumps straight to that page. */
static void android_dot_click_cb(lv_event_t *event)
{
    lv_obj_t *dot = lv_event_get_target(event);
    uintptr_t idx = (uintptr_t)lv_obj_get_user_data(dot);

    if (idx < PAGE_COUNT) {
        lv_tileview_set_tile_by_index(s_tileview, (uint32_t)idx, 0,
                                      LV_ANIM_ON);
    }
}

static void android_nav_bar_create(lv_obj_t *parent)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_set_size(bar, LV_PCT(100), NAV_BAR_H);
    ui_style_surface(bar, UI_COLOR_SURFACE);
    ui_style_flex(bar, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 24);

    static const char *labels[] = {
        LV_SYMBOL_CALL, LV_SYMBOL_ENVELOPE, LV_SYMBOL_HOME
    };

    for (unsigned i = 0; i < sizeof(labels) / sizeof(labels[0]); i++) {
        lv_obj_t *btn = lv_button_create(bar);
        lv_obj_set_size(btn, 44, 44);
        lv_obj_set_style_radius(btn, 14, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, UI_COLOR_TILE, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, UI_COLOR_TILE_PRESS, LV_STATE_PRESSED);
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);

        lv_obj_t *icon = ui_label(btn, labels[i], UI_COLOR_ACCENT, UI_FONT_ICON);
        lv_obj_center(icon);
    }

    /* The only navigation that leaves this view. */
    lv_obj_t *back = lv_button_create(bar);
    lv_obj_set_size(back, 44, 44);
    lv_obj_set_style_radius(back, 14, LV_PART_MAIN);
    lv_obj_set_style_bg_color(back, UI_COLOR_ACCENT, LV_PART_MAIN);
    lv_obj_set_style_bg_color(back, UI_COLOR_ACCENT_2, LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(back, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(back, android_back_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *back_icon = ui_label(back, LV_SYMBOL_LEFT,
                                   UI_COLOR_TEXT, UI_FONT_ICON);
    lv_obj_center(back_icon);
}

void ui_android_init(void)
{
    if (s_root != NULL) {
        return;
    }

    s_root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(s_root, LV_PCT(100), LV_PCT(100));
    ui_style_surface(s_root, UI_COLOR_BG);
    ui_style_flex(s_root, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 0);

    android_status_bar_create(s_root);

    /* Swipeable app pages. */
    s_tileview = lv_tileview_create(s_root);
    lv_obj_set_width(s_tileview, LV_PCT(100));
    lv_obj_set_flex_grow(s_tileview, 1);
    ui_style_surface(s_tileview, UI_COLOR_BG);
    /*
     * ui_style_surface() clears LV_OBJ_FLAG_SCROLLABLE, but a tileview must
     * stay scrollable or swiping between its tiles does nothing. Restore the
     * flag (and the horizontal scroll direction) after styling.
     */
    lv_obj_add_flag(s_tileview, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(s_tileview, LV_DIR_HOR);
    lv_obj_add_event_cb(s_tileview, android_tile_change_cb,
                        LV_EVENT_VALUE_CHANGED, NULL);

    for (int i = 0; i < PAGE_COUNT; i++) {
        s_tiles[i] = lv_tileview_add_tile(s_tileview, i, 0, LV_DIR_HOR);
        android_page_create(s_tiles[i], i + 1);
    }

    /* Page indicator dots. */
    lv_obj_t *indicator = lv_obj_create(s_root);
    lv_obj_set_size(indicator, LV_PCT(100), INDICATOR_H);
    ui_style_surface(indicator, UI_COLOR_BG);
    ui_style_flex(indicator, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 8);

    for (int i = 0; i < PAGE_COUNT; i++) {
        lv_obj_t *dot = lv_button_create(indicator);
        lv_obj_set_size(dot, 22, 22);
        lv_obj_set_style_bg_opa(dot, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_border_width(dot, 0, LV_PART_MAIN);
        lv_obj_set_style_shadow_width(dot, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(dot, 0, LV_PART_MAIN);
        lv_obj_set_user_data(dot, (void *)(uintptr_t)i);
        lv_obj_add_event_cb(dot, android_dot_click_cb, LV_EVENT_CLICKED, NULL);

        s_indicators[i] = lv_obj_create(dot);
        lv_obj_set_size(s_indicators[i], 8, 8);
        lv_obj_set_style_radius(s_indicators[i], LV_PCT(50), LV_PART_MAIN);
        lv_obj_set_style_border_width(s_indicators[i], 0, LV_PART_MAIN);
        lv_obj_center(s_indicators[i]);
        lv_obj_set_style_bg_color(s_indicators[i],
                                  (i == 0) ? UI_COLOR_ACCENT : UI_COLOR_TILE,
                                  LV_PART_MAIN);
    }

    android_nav_bar_create(s_root);
}

void ui_android_open(void)
{
    if (s_root != NULL) {
        lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_android_pause(void)
{
    if (s_root != NULL) {
        lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_android_resume(void)
{
    if (s_root != NULL) {
        lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_android_close(void)
{
    if (s_root != NULL) {
        lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_android_deinit(void)
{
    if (s_root != NULL) {
        lv_obj_del(s_root);
        s_root = NULL;
        s_tileview = NULL;
    }
}
