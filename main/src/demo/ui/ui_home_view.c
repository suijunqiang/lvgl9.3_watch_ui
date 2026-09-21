#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t g_home_page = {0};

static void home_action_cb(lv_event_t *event)
{
    lv_obj_t *button = lv_event_get_target(event);
    view_id_t target = (view_id_t)(uintptr_t)lv_obj_get_user_data(button);
    ui_push_view(target);
}

void view_home_init(void)
{
    if (g_home_page.initialized) {
        return;
    }

    ui_page_create_root(&g_home_page, "HOME", lv_color_hex(0x0D1117));

    lv_obj_t *status_bar = lv_obj_create(g_home_page.root);
    lv_obj_set_size(status_bar, LV_PCT(100), 30);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "9:41");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 16, 0);

    lv_obj_t *wifi_label = lv_label_create(status_bar);
    lv_label_set_text(wifi_label, "WiFi 90%");
    lv_obj_align(wifi_label, LV_ALIGN_RIGHT_MID, -12, 0);

    lv_obj_t *card = lv_obj_create(g_home_page.root);
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

    lv_obj_t *grid = lv_obj_create(g_home_page.root);
    lv_obj_set_size(grid, 260, 180);
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, -18);

    lv_obj_t *btn_music = lv_btn_create(grid);
    lv_obj_set_size(btn_music, 70, 70);
    lv_obj_align(btn_music, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_radius(btn_music, 18, 0);
    lv_obj_set_style_bg_color(btn_music, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_music = lv_label_create(btn_music);
    lv_label_set_text(lab_music, "M");
    lv_obj_center(lab_music);

    lv_obj_t *txt_music = lv_label_create(btn_music);
    lv_label_set_text(txt_music, "Music");
    lv_obj_align(txt_music, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_music, (void *)(uintptr_t)VIEW_MUSIC);
    lv_obj_add_event_cb(btn_music, home_action_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_health = lv_btn_create(grid);
    lv_obj_set_size(btn_health, 70, 70);
    lv_obj_align(btn_health, LV_ALIGN_TOP_LEFT, 90, 0);
    lv_obj_set_style_radius(btn_health, 18, 0);
    lv_obj_set_style_bg_color(btn_health, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_health = lv_label_create(btn_health);
    lv_label_set_text(lab_health, "H");
    lv_obj_center(lab_health);

    lv_obj_t *txt_health = lv_label_create(btn_health);
    lv_label_set_text(txt_health, "Health");
    lv_obj_align(txt_health, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_health, (void *)(uintptr_t)VIEW_WIFI);
    lv_obj_add_event_cb(btn_health, home_action_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_set = lv_btn_create(grid);
    lv_obj_set_size(btn_set, 70, 70);
    lv_obj_align(btn_set, LV_ALIGN_TOP_LEFT, 180, 0);
    lv_obj_set_style_radius(btn_set, 18, 0);
    lv_obj_set_style_bg_color(btn_set, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_set = lv_label_create(btn_set);
    lv_label_set_text(lab_set, "S");
    lv_obj_center(lab_set);

    lv_obj_t *txt_set = lv_label_create(btn_set);
    lv_label_set_text(txt_set, "Set");
    lv_obj_align(txt_set, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_set, (void *)(uintptr_t)VIEW_SETTING);
    lv_obj_add_event_cb(btn_set, home_action_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_notif = lv_btn_create(grid);
    lv_obj_set_size(btn_notif, 70, 70);
    lv_obj_align(btn_notif, LV_ALIGN_TOP_LEFT, 0, 90);
    lv_obj_set_style_radius(btn_notif, 18, 0);
    lv_obj_set_style_bg_color(btn_notif, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_notif = lv_label_create(btn_notif);
    lv_label_set_text(lab_notif, "N");
    lv_obj_center(lab_notif);

    lv_obj_t *txt_notif = lv_label_create(btn_notif);
    lv_label_set_text(txt_notif, "Notif");
    lv_obj_align(txt_notif, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_notif, (void *)(uintptr_t)VIEW_POPUP);
    lv_obj_add_event_cb(btn_notif, home_action_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_app = lv_btn_create(grid);
    lv_obj_set_size(btn_app, 70, 70);
    lv_obj_align(btn_app, LV_ALIGN_TOP_LEFT, 90, 90);
    lv_obj_set_style_radius(btn_app, 18, 0);
    lv_obj_set_style_bg_color(btn_app, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_app = lv_label_create(btn_app);
    lv_label_set_text(lab_app, "A");
    lv_obj_center(lab_app);

    lv_obj_t *txt_app = lv_label_create(btn_app);
    lv_label_set_text(txt_app, "App");
    lv_obj_align(txt_app, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_app, (void *)(uintptr_t)VIEW_ANDROID_LAUNCHER);
    lv_obj_add_event_cb(btn_app, home_action_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_watch = lv_btn_create(grid);
    lv_obj_set_size(btn_watch, 70, 70);
    lv_obj_align(btn_watch, LV_ALIGN_TOP_LEFT, 180, 90);
    lv_obj_set_style_radius(btn_watch, 18, 0);
    lv_obj_set_style_bg_color(btn_watch, lv_color_hex(0x1D2330), 0);

    lv_obj_t *lab_watch = lv_label_create(btn_watch);
    lv_label_set_text(lab_watch, "W");
    lv_obj_center(lab_watch);

    lv_obj_t *txt_watch = lv_label_create(btn_watch);
    lv_label_set_text(txt_watch, "Watch");
    lv_obj_align(txt_watch, LV_ALIGN_BOTTOM_MID, 0, -6);
    lv_obj_set_user_data(btn_watch, (void *)(uintptr_t)VIEW_WATERFALL);
    lv_obj_add_event_cb(btn_watch, home_action_cb, LV_EVENT_CLICKED, NULL);
}

void view_home_open(void)
{
    if (!g_home_page.initialized) {
        view_home_init();
    }
    ui_page_show(&g_home_page);
}

void view_home_pause(void)
{
    ui_page_hide(&g_home_page);
}

void view_home_resume(void)
{
    ui_page_show(&g_home_page);
}

void view_home_close(void)
{
    ui_page_hide(&g_home_page);
}

void view_home_deinit(void)
{
    ui_page_destroy(&g_home_page);
}
