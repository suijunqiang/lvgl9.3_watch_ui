#include "music_view.h"
#include "view_manager.h"

static lv_obj_t * music_view = NULL;

void music_view_create(lv_obj_t * parent)
{
    /* Create the music view object */
    music_view = lv_obj_create(parent);
    lv_obj_set_size(music_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(music_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x1A2456));
    lv_obj_add_style(music_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(music_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

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
}

void music_view_destroy(void)
{
    if(music_view) {
        lv_obj_del(music_view);
        music_view = NULL;
    }
}