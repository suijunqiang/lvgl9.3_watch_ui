#include "gallery_view.h"
#include "view_manager.h"

static lv_obj_t * gallery_view = NULL;

void gallery_view_create(lv_obj_t * parent)
{
    /* Create the gallery view object */
    gallery_view = lv_obj_create(parent);
    lv_obj_set_size(gallery_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(gallery_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x352F44));
    lv_obj_add_style(gallery_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(gallery_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

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
}

void gallery_view_destroy(void)
{
    if(gallery_view) {
        lv_obj_del(gallery_view);
        gallery_view = NULL;
    }
}