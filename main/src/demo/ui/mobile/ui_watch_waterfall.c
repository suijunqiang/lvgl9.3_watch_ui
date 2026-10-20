/*
 * Watch waterfall view.
 *
 * The algorithm is ported from lv_demo_waterfall() (main/src/demo/view_watefall.h)
 * with the image assets removed:
 *
 *   - Icons sit on a staggered ("honeycomb") grid whose rows are offset by
 *     half a cell. Only every second row is shifted, so the pattern looks
 *     like a brick wall.
 *   - Dragging pans the whole grid. Every icon is re-laid out from its
 *     precomputed home position plus the current pan offset.
 *   - Zoom: the closer a tile is to the screen centre, the larger it is.
 *     Scaling uses lv_obj_set_style_transform_scale() where 256 == 100%,
 *     i.e. the tile is real-time scaled, not resized.
 *
 * Tiles are plain rounded LVGL objects showing the same built-in symbol, so
 * the zoom effect works with nothing but the core library.
 *
 * The header (with a working back button) is created by ui_page_base, so
 * this view never gets stuck without a way home.
 *
 * Layer 4 (example view).
 */

#include "../ui_views.h"
#include "../ui_page_base.h"
#include "../ui_theme.h"
#include <math.h>

/* Number of tiles, roughly matching the original honeycomb demo. */
#define ICON_NUM        15
#define GRID_COLS       5
#define ICON_SIZE       70    /* cell size in pixels */

/* Zoom falloff distance: a tile farther than this from the centre keeps
 * the minimum scale. Matches IMAGE_ICON_OFFSET in the original demo. */
#define ICON_FALLOFF    380

/* transform_scale range in LVGL (256 = 100%). The original demo maps the
 * centre to ~240 (94%) and the far edge to ~15 (6%). */
#define SCALE_MIN       15
#define SCALE_MAX       240

/* Pan limit, so the grid can never be dragged completely off-screen. */
#define PAN_LIMIT       380

/* Row stagger: 0.866 is sin(60 deg), the vertical spacing of a hex grid. */
#define ROW_STEP        0.866f

static ui_page_t s_page;
static lv_obj_t *s_canvas;             /* tile container, below header */
static lv_obj_t *s_icons[ICON_NUM];
static lv_point_t s_home[ICON_NUM];    /* precomputed grid position */
static lv_point_t s_pan;               /* current drag offset        */
static bool canvas_sized;              /* grid centred at least once */

static void waterfall_layout(void);
static void waterfall_center_grid(void);

/* ------------------------------------------------------------------ */
/* Zoom                                                                */
/* ------------------------------------------------------------------ */

/*
 * Map a squared screen-centre distance to an LVGL transform scale.
 *
 * `dist2` is dx*dx + dy*dy. The original demo used lv_sqrt() with a fixed
 * point helper; a plain sqrtf() gives the same linear falloff without the
 * extra dependency, and the curve is identical:
 *
 *   t = 1 - dist / falloff        clamped to [0, 1]
 *   scale = SCALE_MIN + t * (SCALE_MAX - SCALE_MIN)
 */
static int zoom_scale_for(int dist2)
{
    float dist = sqrtf((float)dist2);

    if (dist > (float)ICON_FALLOFF) {
        dist = (float)ICON_FALLOFF;
    }

    float t = 1.0f - dist / (float)ICON_FALLOFF;

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    return SCALE_MIN + (int)(t * (float)(SCALE_MAX - SCALE_MIN));
}

/* ------------------------------------------------------------------ */
/* Layout                                                              */
/* ------------------------------------------------------------------ */

static void waterfall_layout(void)
{
    if (s_canvas == NULL) {
        return;
    }

    int w = lv_obj_get_width(s_canvas);
    int h = lv_obj_get_height(s_canvas);

    /* The canvas may not be sized yet; LV_EVENT_SIZE_CHANGED triggers a
     * later update once the display driver reports real dimensions. */
    if (w == 0 || h == 0) {
        return;
    }

    int cx = w / 2;
    int cy = h / 2;

    for (int i = 0; i < ICON_NUM; i++) {
        int pos_x = s_home[i].x + s_pan.x;
        int pos_y = s_home[i].y + s_pan.y;

        int dx = pos_x - cx;
        int dy = pos_y - cy;
        int scale = zoom_scale_for(dx * dx + dy * dy);

        /*
         * transform_scale keeps the widget's layout box unchanged and scales
         * it around its centre, so the tile stays centred on pos_x/pos_y.
         */
        lv_obj_set_style_transform_scale(s_icons[i], scale, 0);
        lv_obj_set_pos(s_icons[i],
                       pos_x - ICON_SIZE / 2,
                       pos_y - ICON_SIZE / 2);

        /* Draw the tiles nearest the centre on top. */
        if (scale > (SCALE_MIN + SCALE_MAX) / 2) {
            lv_obj_move_foreground(s_icons[i]);
        }
    }
}

/*
 * Pan offset that centres the whole staggered grid inside the canvas, so the
 * tiles start in the middle of the view instead of hugging the top-left.
 */
static void waterfall_center_grid(void)
{
    if (s_canvas == NULL) {
        return;
    }

    int w = lv_obj_get_width(s_canvas);
    int h = lv_obj_get_height(s_canvas);

    if (w == 0 || h == 0) {
        return;
    }

    /* Bounding box of the home positions (they are tile centres). */
    int min_x = s_home[0].x, max_x = s_home[0].x;
    int min_y = s_home[0].y, max_y = s_home[0].y;

    for (int i = 1; i < ICON_NUM; i++) {
        if (s_home[i].x < min_x) min_x = s_home[i].x;
        if (s_home[i].x > max_x) max_x = s_home[i].x;
        if (s_home[i].y < min_y) min_y = s_home[i].y;
        if (s_home[i].y > max_y) max_y = s_home[i].y;
    }

    s_pan.x = w / 2 - (min_x + max_x) / 2;
    s_pan.y = h / 2 - (min_y + max_y) / 2;
}

static void pan_by(lv_coord_t dx, lv_coord_t dy)
{
    s_pan.x += dx;
    s_pan.y += dy;

    if (s_pan.x > PAN_LIMIT) s_pan.x = PAN_LIMIT;
    if (s_pan.x < -PAN_LIMIT) s_pan.x = -PAN_LIMIT;
    if (s_pan.y > PAN_LIMIT) s_pan.y = PAN_LIMIT;
    if (s_pan.y < -PAN_LIMIT) s_pan.y = -PAN_LIMIT;

    waterfall_layout();
}

/* ------------------------------------------------------------------ */
/* Events                                                             */
/* ------------------------------------------------------------------ */

static void canvas_size_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_SIZE_CHANGED) {
        canvas_sized = true;
        waterfall_center_grid();
        waterfall_layout();
    }
}

static void canvas_drag_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_PRESSING) {
        return;
    }

    lv_indev_t *indev = lv_indev_active();
    if (indev == NULL) {
        return;
    }

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    pan_by(vect.x, vect.y);
}

static void icon_click_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
        return;
    }

    lv_obj_t *mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Hello");
    lv_msgbox_add_text(mbox, "icon btn click.");
    lv_msgbox_add_close_button(mbox);
}

/* ------------------------------------------------------------------ */
/* View                                                               */
/* ------------------------------------------------------------------ */

static void waterfall_build(ui_page_t *page)
{
    ui_page_create_root(page, "WATCH_WATERFALL", UI_COLOR_BG);

    /*
     * Header with a working back button. page->content is the area below the
     * header; the canvas must live *inside* it (a sibling canvas would fight
     * the header for the root's flex space and end up squeezed).
     */
    lv_obj_t *content = ui_page_create_header(page, "Watch", true);

    s_canvas = lv_obj_create(content);
    lv_obj_set_width(s_canvas, LV_PCT(100));
    lv_obj_set_height(s_canvas, LV_PCT(100));
    lv_obj_set_flex_grow(s_canvas, 1);
    ui_style_surface(s_canvas, UI_COLOR_BG);
    /* Trim the content padding so tiles can run to the edges. */
    lv_obj_set_style_pad_all(content, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_row(content, 0, LV_PART_MAIN);
    lv_obj_add_flag(s_canvas, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(s_canvas, LV_SCROLLBAR_MODE_OFF);
    /* Tiles are positioned with lv_obj_set_pos(), so the canvas must not
     * lay its children out. */
    lv_obj_set_layout(s_canvas, LV_LAYOUT_NONE);
    lv_obj_add_event_cb(s_canvas, canvas_size_event_cb,
                        LV_EVENT_SIZE_CHANGED, NULL);
    lv_obj_add_event_cb(s_canvas, canvas_drag_event_cb, LV_EVENT_PRESSING, NULL);

    s_pan.x = 0;
    s_pan.y = 0;
    canvas_sized = false;

    for (int i = 0; i < ICON_NUM; i++) {
        int col = i % GRID_COLS;
        int row = i / GRID_COLS;

        /* Staggered grid: every odd row is shifted half a cell to the right. */
        int x = col * ICON_SIZE + ICON_SIZE / 2;
        if (row % 2) {
            x += ICON_SIZE / 2;
        }
        int y = (int)(row * ICON_SIZE * ROW_STEP) + ICON_SIZE / 2;

        s_home[i].x = x;
        s_home[i].y = y;

        lv_obj_t *tile = lv_obj_create(s_canvas);
        lv_obj_set_size(tile, ICON_SIZE, ICON_SIZE);
        lv_obj_set_style_radius(tile, ICON_SIZE / 4, LV_PART_MAIN);
        lv_obj_set_style_border_width(tile, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(tile, UI_COLOR_TILE, LV_PART_MAIN);
        lv_obj_add_flag(tile, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_flag(tile, LV_OBJ_FLAG_CLICKABLE);
        /*
         * The drag handler is attached to every tile as well as the canvas:
         * a press landing on a clickable tile would otherwise never bubble a
         * PRESSING event up to the canvas, so the grid would not move.
         */
        lv_obj_add_event_cb(tile, canvas_drag_event_cb, LV_EVENT_PRESSING, NULL);
        lv_obj_add_event_cb(tile, icon_click_event_cb, LV_EVENT_CLICKED, NULL);

        /* Built-in LVGL icon glyph, identical for every tile. */
        lv_obj_t *glyph = ui_label(tile, LV_SYMBOL_IMAGE,
                                   UI_COLOR_ACCENT, UI_FONT_ICON_LARGE);
        lv_obj_center(glyph);

        s_icons[i] = tile;
    }

    waterfall_center_grid();
    waterfall_layout();
}

UI_VIEW_DEFINE(ui_watch_waterfall, &s_page, waterfall_build)
