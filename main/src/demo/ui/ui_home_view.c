/*
 * Home view.
 *
 * A smartwatch style home screen:
 *
 *   ┌──────────────────────────────────┐
 *   │ 9:41        WIFI  BT  [98%]       │  status bar
 *   ├──────────────────────────────────┤
 *   │        ╭──────────────╮           │
 *   │        │    04:21     │           │  clock card
 *   │        │  Mon 12 Oct  │           │
 *   │        │ ◉ 8460   98% │           │
 *   │        ╰──────────────╯           │
 *   │   [♪]   [⚙]   [☰]                │  quick-launch grid
 *   │  Music   Set   Apps               │
 *   └──────────────────────────────────┘
 *
 * Layout uses LVGL flex containers instead of magic pixel offsets, so the
 * screen adapts to the display size. All icons are the built-in LVGL symbol
 * font, so the view needs no image assets.
 *
 * This is the root view, so it has no header / back button.
 *
 * Layer 4 (example view): only builds widgets; lifecycle comes from
 * UI_VIEW_DEFINE().
 */

#include "ui_views.h"
#include "ui_page_base.h"
#include "ui_theme.h"

static ui_page_t s_page;

/* ------------------------------------------------------------------ */
/* Quick-launch tiles                                                  */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *symbol;  /* LV_SYMBOL_* shown on the tile */
    const char *caption; /* label under the tile          */
    view_id_t target;    /* view pushed when tapped       */
} home_tile_t;

static const home_tile_t s_tiles[] = {
    { LV_SYMBOL_AUDIO,    "Music", VIEW_MUSIC            },
    { LV_SYMBOL_SETTINGS, "Set",   VIEW_SETTING          },
    { LV_SYMBOL_LIST,     "Apps",  VIEW_ANDROID_LAUNCHER },
    { LV_SYMBOL_WIFI,     "WiFi",  VIEW_WIFI             },
    { LV_SYMBOL_BELL,     "Popup", VIEW_POPUP            },
    { LV_SYMBOL_IMAGE,    "Watch", VIEW_WATERFALL        },
};

/* ------------------------------------------------------------------ */
/* Status bar                                                          */
/* ------------------------------------------------------------------ */

/*
 * One status item: a small symbol plus an optional value, e.g.
 * "WIFI 98%". Keeps the status bar a plain flex row.
 */
static void status_item_create(lv_obj_t *parent,
                               const char *symbol,
                               const char *value)
{
    lv_obj_t *item = lv_obj_create(parent);
    lv_obj_set_size(item, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(item);
    ui_style_flex(item, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 6);

    ui_label(item, symbol, UI_COLOR_TEXT, UI_FONT_SMALL);

    if (value != NULL) {
        ui_label(item, value, UI_COLOR_TEXT_DIM, UI_FONT_SMALL);
    }
}

static void status_bar_create(lv_obj_t *parent)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_set_size(bar, LV_PCT(100), 34);
    ui_style_surface(bar, UI_COLOR_SURFACE);
    ui_style_flex(bar, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_hor(bar, UI_PAD, LV_PART_MAIN);

    ui_label(bar, "9:41", UI_COLOR_TEXT, UI_FONT_BODY);

    /* Right cluster: connectivity + battery. */
    lv_obj_t *right = lv_obj_create(bar);
    lv_obj_set_size(right, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(right);
    ui_style_flex(right, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 12);

    status_item_create(right, LV_SYMBOL_WIFI, NULL);
    status_item_create(right, LV_SYMBOL_BLUETOOTH, NULL);
    status_item_create(right, LV_SYMBOL_BATTERY_3, "98%");
}

/* ------------------------------------------------------------------ */
/* Clock card                                                          */
/* ------------------------------------------------------------------ */

static void clock_card_create(lv_obj_t *parent)
{
    lv_obj_t *card = ui_card(parent, UI_COLOR_ACCENT);
    lv_obj_set_width(card, LV_PCT(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_grad_color(card, UI_COLOR_ACCENT_2, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(card, LV_GRAD_DIR_HOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(card, 12, LV_PART_MAIN);
    ui_style_flex(card, LV_FLEX_FLOW_ROW,
                  LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, 10);

    /* Left: clock + date. */
    lv_obj_t *left = lv_obj_create(card);
    lv_obj_set_size(left, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(left);
    ui_style_flex(left, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, 0);

    ui_label(left, "04:21", UI_COLOR_TEXT, UI_FONT_TITLE);

    lv_obj_t *date = ui_label(left, "Mon 12 Oct", UI_COLOR_TEXT, UI_FONT_SMALL);
    lv_obj_set_style_text_opa(date, LV_OPA_80, LV_PART_MAIN);

    /* Right: steps + battery, stacked. */
    lv_obj_t *right = lv_obj_create(card);
    lv_obj_set_size(right, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    ui_style_transparent(right);
    ui_style_flex(right, LV_FLEX_FLOW_COLUMN,
                  LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, 2);

    status_item_create(right, LV_SYMBOL_GPS, "8460");
    status_item_create(right, LV_SYMBOL_CHARGE, "98%");
}

/* ------------------------------------------------------------------ */
/* Quick-launch tiles                                                  */
/* ------------------------------------------------------------------ */

static void tile_event_cb(lv_event_t *event)
{
    lv_obj_t *tile = lv_event_get_target(event);
    view_id_t target = (view_id_t)(uintptr_t)lv_obj_get_user_data(tile);

    ui_push_view(target);
}

static void tile_create(lv_obj_t *parent, const home_tile_t *tile)
{
    /*
     * One cell = button on top, caption underneath.
     *
     * The cell has an explicit size and LV_LAYOUT_NONE: as a flex child,
     * lv_button's class defaults (width_def/height_def = LV_SIZE_CONTENT)
     * would otherwise collapse it to the size of its own glyph label. With
     * no layout on the cell the button keeps the explicit box below.
     */
    lv_obj_t *cell = lv_obj_create(parent);
    lv_obj_set_size(cell, UI_TILE_CELL_W, UI_TILE_CELL_H);
    ui_style_transparent(cell);
    lv_obj_set_layout(cell, LV_LAYOUT_NONE);

    lv_obj_t *btn = lv_button_create(cell);
    lv_obj_set_size(btn, UI_TOUCH_SIZE, UI_TOUCH_SIZE);
    lv_obj_set_style_min_width(btn, UI_TOUCH_SIZE, LV_PART_MAIN);
    lv_obj_set_style_min_height(btn, UI_TOUCH_SIZE, LV_PART_MAIN);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_radius(btn, UI_RADIUS_TILE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, UI_COLOR_TILE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, UI_COLOR_TILE_PRESS, LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_user_data(btn, (void *)(uintptr_t)tile->target);
    lv_obj_add_event_cb(btn, tile_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *glyph = ui_label(btn, tile->symbol,
                               UI_COLOR_ACCENT, UI_FONT_ICON_LARGE);
    lv_obj_center(glyph);

    lv_obj_t *caption = ui_label(cell, tile->caption,
                                 UI_COLOR_TEXT_DIM, UI_FONT_SMALL);
    lv_obj_align(caption, LV_ALIGN_BOTTOM_MID, 0, 0);
}

/* ------------------------------------------------------------------ */
/* Build                                                              */
/* ------------------------------------------------------------------ */

static void home_build(ui_page_t *page)
{
    ui_page_create_root(page, "HOME", UI_COLOR_BG);

    /* Root view: status bar flush at the top, then a content column. */
    lv_obj_t *root = page->root;

    status_bar_create(root);

    lv_obj_t *content = lv_obj_create(root);
    lv_obj_set_width(content, LV_PCT(100));
    lv_obj_set_flex_grow(content, 1);
    ui_style_transparent(content);
    lv_obj_set_style_pad_all(content, UI_PAD, LV_PART_MAIN);
    lv_obj_set_style_pad_row(content, UI_GAP, LV_PART_MAIN);
    lv_obj_remove_flag(content, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    page->content = content;

    lv_obj_t *card_wrap = lv_obj_create(content);
    lv_obj_set_width(card_wrap, LV_PCT(100));
    lv_obj_set_height(card_wrap, LV_SIZE_CONTENT);
    ui_style_transparent(card_wrap);
    clock_card_create(card_wrap);

    /* Grid: three launcher cells per row (UI_GRID_COLS). */
    lv_obj_t *grid = lv_obj_create(content);
    lv_obj_set_width(grid, LV_PCT(100));
    lv_obj_set_height(grid, LV_SIZE_CONTENT);
    ui_style_transparent(grid);

    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid,
                          LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(grid, 12, LV_PART_MAIN);

    for (unsigned i = 0; i < sizeof(s_tiles) / sizeof(s_tiles[0]); i++) {
        tile_create(grid, &s_tiles[i]);
    }
}

UI_VIEW_DEFINE(view_home, &s_page, home_build)
