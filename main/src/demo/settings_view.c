#include "settings_view.h"
#include "view_manager.h"

static lv_obj_t * settings_view = NULL;

void settings_view_create(lv_obj_t * parent)
{
    /* Create the settings view object */
    settings_view = lv_obj_create(parent);
    lv_obj_set_size(settings_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(settings_view, LV_OBJ_FLAG_SCROLLABLE);

    /* Set the style */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x1E1E1E));
    lv_obj_add_style(settings_view, &style, 0);

    /* Back button */
    lv_obj_t * back_btn = lv_btn_create(settings_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* Title */
    lv_obj_t * title = lv_label_create(settings_view);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* TileView */
    lv_obj_t * tileview = lv_tileview_create(settings_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* Set the size and enable elastic scrolling */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100% width
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* Create 3 tiles */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* First tile: System Settings */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile1, LV_OBJ_FLAG_SCROLLABLE);

    /* Tile label */
    lv_obj_t * label1 = lv_label_create(tile1);
    lv_label_set_text(label1, "System Settings");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_18, 0);
    lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 20);

    /* Second tile: Network Settings */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * label2 = lv_label_create(tile2);
    lv_label_set_text(label2, "Network Settings");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_18, 0);
    lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 20);

    /* Third tile: About */
    lv_obj_t * tile3 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile3, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * label3 = lv_label_create(tile3);
    lv_label_set_text(label3, "About");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_18, 0);
    lv_obj_align(label3, LV_ALIGN_TOP_MID, 0, 20);

    /* Set the initial tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);
}

void settings_view_destroy(void)
{
    if(settings_view) {
        lv_obj_del(settings_view);
        settings_view = NULL;
    }
}