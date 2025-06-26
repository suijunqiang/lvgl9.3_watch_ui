#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include "../../../lvgl/lvgl.h"

#define VIEW_MANAGER_SCW 320
#define VIEW_MANAGER_SCH 320
/* View ID */
typedef enum {
    VIEW_MAIN_E,
    VIEW_SETTINGS_E,
    VIEW_MUSIC_E,
    VIEW_GALLERY_E,
    VIEW_COUNT_E
} view_id_enum;

/* View callback function types */
typedef void (*view_create_cb_t)(lv_obj_t * parent);
typedef void (*view_destroy_cb_t)(void);

/* Current active view */
static view_id_enum top_view = VIEW_MAIN_E;
/* Main view object */
static lv_obj_t * main_view_bk   = NULL;

/* View manager initialization */
void manager_init(void);

/* View creation functions */
static void create_main(lv_obj_t * parent);
static void create_settings(lv_obj_t * parent);
static void create_music(lv_obj_t * parent);
static void create_gallery(lv_obj_t * parent);

/* View destruction functions */
static void destroy_main(void);
static void destroy_settings(void);
static void destroy_music(void);
static void destroy_gallery(void);

/* View switching functions */
static void switch_to(view_id_enum view_id);
static void return_to_main(lv_event_t * e);

/* Icon click event handler */
static void img_click_event_handler(lv_event_t * e);

/* Swipe event handler */
static void swipe_event_handler(lv_event_t * e);

/* View registration table */
static struct {
    view_create_cb_t create_cb;
    view_destroy_cb_t destroy_cb;
    lv_obj_t * obj;
} views[VIEW_COUNT_E] = {
    [VIEW_MAIN_E]     = {create_main, destroy_main, NULL},
    [VIEW_SETTINGS_E] = {create_settings, destroy_settings, NULL},
    [VIEW_MUSIC_E]    = {create_music, destroy_music, NULL},
    [VIEW_GALLERY_E]  = {create_gallery, destroy_gallery, NULL}
};

/* Example function */
void lv_example_view_manager(void) {
    manager_init();
}

/* View manager initialization */
void manager_init(void) {
    /* Switch to the main view */
    switch_to(VIEW_MAIN_E);
}

/* Switch to the specified view */
static void switch_to(view_id_enum view_id) {
    printf("switch_to_view \n");
    /* Destroy the current view if it's not the main view */
    if(top_view != VIEW_MAIN_E && views[top_view].destroy_cb) {
        printf("in top_view if\n");
        views[top_view].destroy_cb();
    }

    /* Update the current view ID */
    top_view = view_id;

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
static void return_to_main(lv_event_t * e) {
    switch_to(VIEW_MAIN_E);
}

/* Icon click event handler */
static void img_click_event_handler(lv_event_t * e) {
    printf("img_click_event_handler\n");
    view_id_enum view_id = (view_id_enum)lv_event_get_user_data(e);
    switch_to(view_id);
}

/* Swipe event handler */
static void swipe_event_handler(lv_event_t * e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_RIGHT && top_view != VIEW_MAIN_E) {
        printf("Swipe right return to main view\n");
        return_to_main(e);
    }
}

/* Create the main view */
static void create_main(lv_obj_t * parent) {
    /* Create the main view object */
    main_view_bk = lv_obj_create(parent);
    lv_obj_set_size(main_view_bk, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_view_bk, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the main view style */
    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_color(&style_main, lv_color_hex(0xFFFFFF));
    lv_obj_add_style(main_view_bk, &style_main, 0);

    /* Create the title */
    lv_obj_t * title = lv_label_create(main_view_bk);
    lv_label_set_text(title, "Main Menu");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* Create the icon grid */
    lv_obj_t * icon_grid = lv_obj_create(main_view_bk);
    lv_obj_set_size(icon_grid, LV_PCT(100), LV_PCT(100));
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

    view_id_enum view_ids[] = {
        VIEW_SETTINGS_E,
        VIEW_MUSIC_E,
        VIEW_GALLERY_E
    };

    /* Create icon buttons */
    for(int i = 0; i < 3; i++) {
        int row = i / 3;
        int col = i % 3;

        /* Create the button */
        lv_obj_t * btn = lv_btn_create(icon_grid);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1,
                              LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(btn, 80, 80);

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
        lv_label_set_text_fmt(label, "  %s\n%s", icons[i], labels[i]);
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

        /* Add the click event */
        lv_obj_add_event_cb(btn, img_click_event_handler, LV_EVENT_CLICKED, (void*)view_ids[i]);
    }

    views[VIEW_MAIN_E].obj = main_view_bk;
}

/* Destroy the main view */
static void destroy_main(void) {
    if(main_view_bk) {
        lv_obj_add_flag(main_view_bk, LV_OBJ_FLAG_HIDDEN);
        main_view_bk = NULL;
        views[VIEW_MAIN_E].obj = NULL;
        // View destroyed, reset state
    }
}

/* Create the settings view */
static void create_settings(lv_obj_t * parent) {
    /* Create the settings view object */
    lv_obj_t * settings_view = lv_obj_create(parent);
    lv_obj_set_size(settings_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(settings_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0xFFFFFF));
    lv_obj_add_style(settings_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(settings_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main, LV_EVENT_CLICKED, NULL);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* Title */
    lv_obj_t * title = lv_label_create(settings_view);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    /* TileView */
    lv_obj_t * tv = lv_tileview_create(settings_view);
    lv_obj_set_size(tv, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tv, LV_ALIGN_BOTTOM_MID, 0, 0);

    /*Tile1: just a label*/
    lv_obj_t * tile1 = lv_tileview_add_tile(tv, 0, 0, LV_DIR_BOTTOM);
    lv_obj_t * label = lv_label_create(tile1);
    lv_label_set_text(label, "Scroll down");
    lv_obj_center(label);

    /*Tile2: a button*/
    lv_obj_t * tile2 = lv_tileview_add_tile(tv, 0, 1, (lv_dir_t)(LV_DIR_TOP | LV_DIR_RIGHT));
    lv_obj_t * btn   = lv_button_create(tile2);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Scroll up or right");

    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(btn);

    /*Tile3: a list*/
    lv_obj_t * tile3 = lv_tileview_add_tile(tv, 1, 1, (lv_dir_t)(LV_DIR_LEFT| LV_DIR_RIGHT));
    lv_obj_t * label3 = lv_label_create(tile3);

    lv_label_set_text(label3, "label");
    lv_obj_center(label3);


    /*Tile4: a list*/
    lv_obj_t * tile4 = lv_tileview_add_tile(tv, 2, 1, (lv_dir_t)(LV_DIR_LEFT|LV_DIR_RIGHT));
    lv_obj_t * list  = lv_list_create(tile4);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));

    lv_list_add_button(list, NULL, "One");
    lv_list_add_button(list, NULL, "Two");
    lv_list_add_button(list, NULL, "Three");
    lv_list_add_button(list, NULL, "Four");
    lv_list_add_button(list, NULL, "Five");
    lv_list_add_button(list, NULL, "Six");
    lv_list_add_button(list, NULL, "Seven");
    lv_list_add_button(list, NULL, "Eight");
    lv_list_add_button(list, NULL, "Nine");
    lv_list_add_button(list, NULL, "Ten");

    /* Add swipe event handler */
    lv_obj_add_event_cb(settings_view, swipe_event_handler, LV_EVENT_GESTURE, NULL);

    /* Set the view object */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS_E].obj = settings_view;
}

/* Destroy the settings view */
static void destroy_settings(void) {
    lv_obj_t * settings_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS_E].obj;
    if(settings_view) {
        lv_obj_del(settings_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS_E].obj = NULL;
    }
}

/* Create the music view */
static void create_music(lv_obj_t * parent) {
    /* Create the music view object */
    lv_obj_t * music_view = lv_obj_create(parent);
    lv_obj_set_size(music_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(music_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0xFFFFF1));
    lv_obj_add_style(music_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(music_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* Title */
    lv_obj_t * title = lv_label_create(music_view);
    lv_label_set_text(title, "Music Player");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* TileView */
    lv_obj_t * tileview = lv_tileview_create(music_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* Set the size and enable elastic scrolling */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100% width
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* Create 2 tiles */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* First tile: Playlist */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* Add the song list */
    lv_obj_t * list = lv_list_create(tile1);
    lv_obj_set_size(list, LV_PCT(90), LV_PCT(90));
    lv_obj_center(list);

    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 1");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 2");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 3");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 4");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 5");

    /* Second tile: Play controls */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* Album cover */
    lv_obj_t * album_cover = lv_obj_create(tile2);
    lv_obj_set_size(album_cover, 150, 150);
    lv_obj_align(album_cover, LV_ALIGN_TOP_MID, 0, 20);

    static lv_style_t style_cover;
    lv_style_init(&style_cover);
    lv_style_set_bg_color(&style_cover, lv_color_hex(0x3A4476));
    lv_style_set_radius(&style_cover, LV_RADIUS_CIRCLE);
    lv_obj_add_style(album_cover, &style_cover, 0);

    /* Control buttons */
    lv_obj_t * btn_prev = lv_btn_create(tile2);
    lv_obj_set_size(btn_prev, 40, 40);
    lv_obj_align(btn_prev, LV_ALIGN_BOTTOM_LEFT, 40, -40);
    lv_obj_t * label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, LV_SYMBOL_PREV);
    lv_obj_center(label_prev);

    lv_obj_t * btn_play = lv_btn_create(tile2);
    lv_obj_set_size(btn_play, 60, 60);
    lv_obj_align(btn_play, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_t * label_play = lv_label_create(btn_play);
    lv_label_set_text(label_play, LV_SYMBOL_PLAY);
    lv_obj_center(label_play);

    lv_obj_t * btn_next = lv_btn_create(tile2);
    lv_obj_set_size(btn_next, 40, 40);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -40, -40);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, LV_SYMBOL_NEXT);
    lv_obj_center(label_next);

    /* Set the initial tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);

    /* Add swipe event handler */
    lv_obj_add_event_cb(music_view, swipe_event_handler, LV_EVENT_GESTURE, NULL);

    /* Set the view object */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC_E].obj = music_view;
}

/* Destroy the music view */
static void destroy_music(void) {
    lv_obj_t * music_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC_E].obj;
    if(music_view) {
        lv_obj_del(music_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC_E].obj = NULL;
    }
}

/* Create the gallery view */
static void create_gallery(lv_obj_t * parent) {
    /* Create the gallery view object */
    lv_obj_t * gallery_view = lv_obj_create(parent);
    lv_obj_set_size(gallery_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(gallery_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0xFFFFFF));
    lv_obj_add_style(gallery_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(gallery_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* Title */
    lv_obj_t * title = lv_label_create(gallery_view);
    lv_label_set_text(title, "Gallery");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* TileView */
    lv_obj_t * tileview = lv_tileview_create(gallery_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* Set the size and enable elastic scrolling */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100% width
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* Create 2 tiles */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* First tile: Thumbnails */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* Image grid */
    lv_obj_t * img_grid = lv_obj_create(tile1);
    lv_obj_set_size(img_grid, LV_PCT(90), LV_PCT(90));
    lv_obj_center(img_grid);

    static lv_coord_t grid_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t grid_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(img_grid, grid_col_dsc, grid_row_dsc);

    /* Show thumbnails */
    for(int i = 0; i < 6; i++) {
        int row = i / 2;
        int col = i % 2;

        lv_obj_t * img_cont = lv_obj_create(img_grid);
        lv_obj_set_grid_cell(img_cont, LV_GRID_ALIGN_CENTER, col, 1,
                            LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(img_cont, 120, 80);

        static lv_style_t style_img;
        lv_style_init(&style_img);
        lv_style_set_bg_color(&style_img, lv_color_hex(0x5C5470));
        lv_style_set_radius(&style_img, 10);
        lv_obj_add_style(img_cont, &style_img, 0);

        lv_obj_t * img_label = lv_label_create(img_cont);
        lv_label_set_text_fmt(img_label, "Image %d", i + 1);
        lv_obj_center(img_label);
    }

    /* Second tile: Image viewer */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* Image view */
    lv_obj_t * img_view = lv_obj_create(tile2);
    lv_obj_set_size(img_view, LV_PCT(90), LV_PCT(70));
    lv_obj_align(img_view, LV_ALIGN_TOP_MID, 0, 20);

    static lv_style_t style_img_view;
    lv_style_init(&style_img_view);
    lv_style_set_bg_color(&style_img_view, lv_color_hex(0x5C5470));
    lv_style_set_radius(&style_img_view, 10);
    lv_obj_add_style(img_view, &style_img_view, 0);

    lv_obj_t * img_view_label = lv_label_create(img_view);
    lv_label_set_text(img_view_label, "Image Preview");
    lv_obj_center(img_view_label);

    /* Image control buttons */
    lv_obj_t * btn_prev = lv_btn_create(tile2);
    lv_obj_set_size(btn_prev, 40, 40);
    lv_obj_align(btn_prev, LV_ALIGN_BOTTOM_LEFT, 40, -20);
    lv_obj_t * label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, LV_SYMBOL_PREV);
    lv_obj_center(label_prev);

    lv_obj_t * btn_next = lv_btn_create(tile2);
    lv_obj_set_size(btn_next, 40, 40);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -40, -20);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, LV_SYMBOL_NEXT);
    lv_obj_center(label_next);

    /* Set the initial tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);

    /* Add swipe event handler */
    lv_obj_add_event_cb(gallery_view, swipe_event_handler, LV_EVENT_GESTURE, NULL);

    /* Set the view object */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY_E].obj = gallery_view;
}

/* Destroy the gallery view */
static void destroy_gallery(void) {
    lv_obj_t * gallery_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY_E].obj;
    if(gallery_view) {
        lv_obj_del(gallery_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY_E].obj = NULL;
    }
}           

#endif // VIEW_MANAGER_H