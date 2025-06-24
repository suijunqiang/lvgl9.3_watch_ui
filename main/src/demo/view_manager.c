#include "view_manager.h"
#include "settings_view.h"
#include "music_view.h"
#include "gallery_view.h"

view_id_t current_view = VIEW_MAIN;
static lv_obj_t * main_view = NULL;

/* View registration table */
static struct {
    view_create_cb_t create_cb;
    view_destroy_cb_t destroy_cb;
    lv_obj_t * obj;
} views[VIEW_COUNT] = {
    [VIEW_MAIN] = {NULL, NULL, NULL},  // Main view handled separately
    [VIEW_SETTINGS] = {settings_view_create, settings_view_destroy, NULL},
    [VIEW_MUSIC] = {music_view_create, music_view_destroy, NULL},
    [VIEW_GALLERY] = {gallery_view_create, gallery_view_destroy, NULL}
};

/* Initialize the view manager */
void view_manager_init(void)
{
    /* Switch to the main view */
    switch_to_view(VIEW_MAIN);
}

/* Switch to the specified view */
void switch_to_view(view_id_t view_id)
{
    printf("switch_to_view \n");
    
    /* Destroy the current view if it's not the main view */
    if(current_view != VIEW_MAIN && views[current_view].destroy_cb) {
        printf("in current_view if\n");
        views[current_view].destroy_cb();
    }

    /* Update the current view ID */
    current_view = view_id;

    /* Create the target view if it doesn't exist */
    if(!views[view_id].obj && views[view_id].create_cb) {
        printf("in views create_cb if\n");
        lv_obj_t * parent = lv_scr_act();
        views[view_id].create_cb(parent);
    }

    /* Show the target view */
    if(views[view_id].obj) {
        printf("in lv_obj_move_foreground if\n");
        lv_obj_clear_flag(views[view_id].obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(views[view_id].obj);
    } else {
        printf("Error: View object is NULL for view_id: %d\n", view_id);
    }
}

/* Callback function to return to the main view */
static void return_to_main_view(lv_event_t * e)
{
    switch_to_view(VIEW_MAIN);
}

/* Icon click event handler */
static void icon_click_event_handler(lv_event_t * e)
{
    printf("icon_click_event_handler\n");
    view_id_t view_id = (view_id_t)lv_event_get_user_data(e);
    switch_to_view(view_id);
}

/* Create the main view */
static void create_main_view(lv_obj_t * parent)
{
    /* Create the main view object */
    main_view = lv_obj_create(parent);
    lv_obj_set_size(main_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the main view style */
    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_color(&style_main, lv_color_hex(0x202020));
    lv_obj_add_style(main_view, &style_main, 0);

    /* Create the title */
    lv_obj_t * title = lv_label_create(main_view);
    lv_label_set_text(title, "Main Menu");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* Create the icon grid */
    lv_obj_t * icon_grid = lv_obj_create(main_view);
    lv_obj_set_size(icon_grid, LV_PCT(90), LV_PCT(70));
    lv_obj_align(icon_grid, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(icon_grid, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the grid columns and rows */
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(icon_grid, col_dsc, row_dsc);

    /* Icons and labels */
    const char * icons[] = {
        LV_SYMBOL_SETTINGS,
        LV_SYMBOL_PLAY,
        LV_SYMBOL_IMAGE
    };

    const char * labels[] = {
        "Settings",
        "Music",
        "Gallery"
    };

    view_id_t view_ids[] = {
        VIEW_SETTINGS,
        VIEW_MUSIC,
        VIEW_GALLERY
    };

    /* Create icon buttons */
    for(int i = 0; i < 3; i++) {
        int row = i / 3;
        int col = i % 3;

        /* Create the button */
        lv_obj_t * btn = lv_btn_create(icon_grid);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1,
                              LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(btn, 100, 100);

        /* Set the button style */
        static lv_style_t style_btn;
        lv_style_init(&style_btn);
        lv_style_set_bg_color(&style_btn, lv_color_hex(0x303030));
        lv_style_set_bg_opa(&style_btn, LV_OPA_80);
        lv_style_set_radius(&style_btn, 20);
        lv_style_set_border_width(&style_btn, 2);
        lv_style_set_border_color(&style_btn, lv_color_hex(0x505050));
        lv_obj_add_style(btn, &style_btn, 0);

        /* Button pressed style */
        static lv_style_t style_btn_pr;
        lv_style_init(&style_btn_pr);
        lv_style_set_bg_color(&style_btn_pr, lv_color_hex(0x404040));
        lv_obj_add_style(btn, &style_btn_pr, LV_STATE_PRESSED);

        /* Create the label */
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "%s\n%s", icons[i], labels[i]);
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

        /* Add the click event */
        lv_obj_add_event_cb(btn, icon_click_event_handler, LV_EVENT_CLICKED, (void*)view_ids[i]);
    }

    views[VIEW_MAIN].obj = main_view;
}

/* Destroy the main view */
static void destroy_main_view(void)
{
    if(main_view) {
        lv_obj_add_flag(main_view, LV_OBJ_FLAG_HIDDEN);
        main_view = NULL;
        views[VIEW_MAIN].obj = NULL;
        // View destroyed, reset state
    }
}