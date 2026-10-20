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
 *   - Zoom: the closer an icon is to the screen centre, the larger it is.
 *     Scaling uses lv_obj_set_style_transform_scale() where 256 == 100%,
 *     i.e. the icon is real-time scaled, not resized.
 *
 * Icons are plain coloured LVGL objects (no images / no flash assets), so
 * the zoom effect works with nothing but the core library.
 *
 * Layer 4 (example view): the widget lifecycle is driven by UI_VIEW_DEFINE().
 */

#include "../ui_views.h"
#include "../ui_page_base.h"
#include <math.h>

/* Number of tiles, roughly matching the original honeycomb demo. */
#define ICON_NUM        15
#define GRID_COLS       5
#define ICON_SIZE       70    /* cell size in pixels */

/* Zoom falloff distance: an icon farther than this from the centre keeps
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
static lv_obj_t *s_icons[ICON_NUM];
static lv_point_t s_home[ICON_NUM];   /* precomputed grid position */
static lv_point_t s_pan;              /* current drag offset        */

static void waterfall_layout(void);

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
    if (!s_page.initialized) {
        return;
    }

    int w = lv_obj_get_width(s_page.root);
    int h = lv_obj_get_height(s_page.root);

    /* The root may not be sized yet; LV_EVENT_SIZE_CHANGED triggers a
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
         * it around its centre, so the icon stays centred on pos_x/pos_y.
         * The half-pixel nudge mirrors the original demo, which compensated
         * for its top-left box placement; harmless here.
         */
        lv_obj_set_style_transform_scale(s_icons[i], scale, 0);
        lv_obj_set_pos(s_icons[i],
                       pos_x - ICON_SIZE / 2,
                       pos_y - ICON_SIZE / 2);

        /* Draw the icons nearest the centre on top. */
        if (scale > (SCALE_MIN + SCALE_MAX) / 2) {
            lv_obj_move_foreground(s_icons[i]);
        }
    }
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

static void root_size_event_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_SIZE_CHANGED) {
        waterfall_layout();
    }
}

static void root_drag_event_cb(lv_event_t *event)
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
    ui_page_create_root(page, "WATCH_WATERFALL", lv_color_hex(0x202020));

    lv_obj_add_event_cb(page->root, root_size_event_cb, LV_EVENT_SIZE_CHANGED, NULL);
    lv_obj_add_event_cb(page->root, root_drag_event_cb, LV_EVENT_PRESSING, NULL);

    s_pan.x = 0;
    s_pan.y = 0;

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

        lv_obj_t *icon = lv_obj_create(page->root);
        lv_obj_set_size(icon, ICON_SIZE, ICON_SIZE);
        lv_obj_set_style_radius(icon, ICON_SIZE / 4, 0);
        lv_obj_set_style_border_width(icon, 0, 0);
        lv_obj_set_style_bg_color(icon, lv_color_hex(0x4F8CFF), 0);
        lv_obj_add_flag(icon, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_flag(icon, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(icon, icon_click_event_cb, LV_EVENT_CLICKED, NULL);

        /* Built-in LVGL icon glyph, identical for every tile. */
        lv_obj_t *glyph = lv_label_create(icon);
        lv_label_set_text(glyph, LV_SYMBOL_IMAGE);
        lv_obj_set_style_text_color(glyph, lv_color_white(), 0);
        lv_obj_center(glyph);

        s_icons[i] = icon;
    }

    waterfall_layout();
}

UI_VIEW_DEFINE(ui_watch_waterfall, &s_page, waterfall_build)
