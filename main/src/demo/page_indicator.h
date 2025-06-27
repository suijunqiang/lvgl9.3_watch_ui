#ifndef PAGE_INDICATOR_H
#define PAGE_INDICATOR_H

#include "../../../lvgl/lvgl.h"

#include "lvgl/lvgl.h"

/* Create pagination indicator */
lv_obj_t *create_pagination_indicator(lv_obj_t * parent, uint8_t dot_count)
{
    /* Create indicator container */
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    
    /* Create dots */
    for (uint8_t i = 0; i < dot_count; i++) {
        lv_obj_t *dot = lv_obj_create(cont);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, 8, 8);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        
        /* Set default inactive state */
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
        
        /* Set spacing between dots */
        if (i > 0) {
            lv_obj_set_style_margin_left(dot, 4, 0);
        }
    }
    
    /* Activate first dot */
    if (dot_count > 0) {
        lv_obj_t *dot = lv_obj_get_child(cont, 0);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    }
    
    return cont;
}

/* Update indicator state */
void update_pagination_indicator(lv_obj_t *indicator, uint8_t active_dot)
{
    if (!indicator) return;
    
    uint8_t child_cnt = lv_obj_get_child_cnt(indicator);
    
    /* Reset all dots to inactive state */
    for (uint8_t i = 0; i < child_cnt; i++) {
        lv_obj_t *dot = lv_obj_get_child(indicator, i);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
    }
    
    /* Set current dot to active state */
    if (active_dot < child_cnt) {
        lv_obj_t *dot = lv_obj_get_child(indicator, active_dot);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    }
}

/* Tileview event callback */
static void tileview_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tileview = lv_event_get_target(e);
    lv_obj_t *indicator = lv_event_get_user_data(e);
    
    if (code == LV_EVENT_VALUE_CHANGED) {
        /* Get current active tile */
        lv_obj_t *act_tile = lv_tileview_get_tile_act(tileview);
        if (!act_tile) return;
        
        /* Get tileview scroll position */
        lv_coord_t scroll_x = lv_obj_get_scroll_x(tileview);
        
        /* Get tile width (assuming all tiles are same width) */
        lv_coord_t tile_width = lv_obj_get_width(act_tile);
        
        /* Calculate current tile index */
        uint8_t tile_idx = scroll_x / tile_width;
        
        /* Update indicator */
        update_pagination_indicator(indicator, tile_idx);
    }
}

/* Create tileview with pagination indicator */
void create_tileview_with_indicator(void)
{
    /* Create tileview */
    lv_obj_t *tileview = lv_tileview_create(lv_scr_act());
    lv_obj_set_size(tileview, lv_pct(100), lv_pct(100));
    
    /* Hide default scrollbars */
    lv_obj_set_scrollbar_mode(tileview, LV_SCROLLBAR_MODE_OFF);
    
    /* Create 3 horizontal tiles */
    lv_obj_t *tile1 = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_HOR);
    lv_obj_t *tile2 = lv_tileview_add_tile(tileview, 1, 0, LV_DIR_HOR);
    lv_obj_t *tile3 = lv_tileview_add_tile(tileview, 2, 0, LV_DIR_HOR);
    
    /* Set tile content */
    lv_obj_t *label1 = lv_label_create(tile1);
    lv_label_set_text(label1, "Tile 1");
    lv_obj_center(label1);
    
    lv_obj_t *label2 = lv_label_create(tile2);
    lv_label_set_text(label2, "Tile 2");
    lv_obj_center(label2);
    
    lv_obj_t *label3 = lv_label_create(tile3);
    lv_label_set_text(label3, "Tile 3");
    lv_obj_center(label3);
    
    /* Create pagination indicator */
    lv_obj_t *indicator = create_pagination_indicator(lv_scr_act(), 3);
    if (indicator) {
        /* Position indicator at bottom */
        lv_obj_align(indicator, LV_ALIGN_BOTTOM_MID, 0, -10);
        
        /* Add event callback */
        lv_obj_add_event_cb(tileview, tileview_event_cb, LV_EVENT_VALUE_CHANGED, indicator);
        
        /* Initialize with first tile active */
        update_pagination_indicator(indicator, 0);
    }
}


#endif /* _PAGE_INDICATOR_H */
