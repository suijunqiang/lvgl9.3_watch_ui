#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

/* A quick-launch tile of the home grid. */
typedef struct {
    const char *glyph;   /* short text drawn on the tile */
    const char *caption; /* label under the glyph        */
    view_id_t target;    /* view pushed when tapped      */
    lv_coord_t x;
    lv_coord_t y;
} home_tile_t;

static const home_tile_t s_tiles[] = {
    { "M", "Music",  VIEW_MUSIC,             0,   0 },
    { "H", "Health", VIEW_WIFI,             90,   0 },
    { "S", "Set",    VIEW_SETTING,         180,   0 },
    { "N", "Notif",  VIEW_POPUP,             0,  90 },
    { "A", "App",    VIEW_ANDROID_LAUNCHER, 90,  90 },
    { "W", "Watch",  VIEW_WATERFALL,       180,  90 },
};

static void tile_event_cb(lv_event_t *event)
{
    lv_obj_t *tile = lv_event_get_target(event);
    view_id_t target = (view_id_t)(uintptr_t)lv_obj_get_user_data(tile);

    ui_push_view(target);
}

static void tile_create(lv_obj_t *parent, const home_tile_t *tile)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 70, 70);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, tile->x, tile->y);
    lv_obj_set_style_radius(btn, 18, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1D2330), 0);
    lv_obj_set_user_data(btn, (void *)(uintptr_t)tile->target);
    lv_obj_add_event_cb(btn, tile_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *glyph = lv_label_create(btn);
    lv_label_set_text(glyph, tile->glyph);
    lv_obj_center(glyph);

    lv_obj_t *caption = lv_label_create(btn);
    lv_label_set_text(caption, tile->caption);
    lv_obj_align(caption, LV_ALIGN_BOTTOM_MID, 0, -6);
}

static void status_bar_create(lv_obj_t *parent)
{
    lv_obj_t *status_bar = lv_obj_create(parent);
    lv_obj_set_size(status_bar, LV_PCT(100), 30);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "9:41");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 16, 0);

    lv_obj_t *wifi_label = lv_label_create(status_bar);
    lv_label_set_text(wifi_label, "WiFi 90%");
    lv_obj_align(wifi_label, LV_ALIGN_RIGHT_MID, -12, 0);
}

static void clock_card_create(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 220, 180);
    lv_obj_align(card, LV_ALIGN_TOP_MID, 0, 52);
    lv_obj_set_style_radius(card, 28, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x1C2332), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_90, 0);

    lv_obj_t *lab_clock = lv_label_create(card);
    lv_label_set_text(lab_clock, "04:21");
    lv_obj_align(lab_clock, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t *lab_date = lv_label_create(card);
    lv_label_set_text(lab_date, "Mon 12 Oct");
    lv_obj_align(lab_date, LV_ALIGN_TOP_MID, 0, 62);

    lv_obj_t *lab_steps = lv_label_create(card);
    lv_label_set_text(lab_steps, "Steps 8460");
    lv_obj_align(lab_steps, LV_ALIGN_BOTTOM_LEFT, 20, -22);

    lv_obj_t *lab_battery = lv_label_create(card);
    lv_label_set_text(lab_battery, "98%");
    lv_obj_align(lab_battery, LV_ALIGN_BOTTOM_RIGHT, -20, -22);
}

static void home_build(ui_page_t *page)
{
    ui_page_create_root(page, "HOME", lv_color_hex(0x0D1117));

    status_bar_create(page->root);
    clock_card_create(page->root);

    lv_obj_t *grid = lv_obj_create(page->root);
    lv_obj_set_size(grid, 260, 180);
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, -18);

    for (unsigned i = 0; i < sizeof(s_tiles) / sizeof(s_tiles[0]); i++) {
        tile_create(grid, &s_tiles[i]);
    }
}

UI_VIEW_DEFINE(view_home, &s_page, home_build)
