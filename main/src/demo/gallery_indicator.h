#ifndef LVGL_GALLERY_DEMO_H
#define LVGL_GALLERY_DEMO_H

#include "lvgl/lvgl.h"

/* ========== 修复：给画廊专属的分页函数加前缀，避免和page_indicator.h重定义 ========== */
lv_obj_t *gallery_create_pagination_indicator(lv_obj_t * parent, uint8_t dot_count)
{
    /* Create indicator container */
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    
    /* Create indicator dots */
    for (uint8_t i = 0; i < dot_count; i++) {
        lv_obj_t *dot = lv_obj_create(cont);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, 8, 8);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        
        /* Default inactive state */
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
        
        /* Spacing between dots */
        if (i > 0) {
            lv_obj_set_style_margin_left(dot, 4, 0);
        }
    }
    
    /* Activate first dot by default */
    if (dot_count > 0) {
        lv_obj_t *dot = lv_obj_get_child(cont, 0);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    }
    
    return cont;
}

void gallery_update_pagination_indicator(lv_obj_t *indicator, uint8_t active_dot)
{
    if (!indicator) return;
    
    uint8_t child_cnt = lv_obj_get_child_cnt(indicator);
    
    /* Reset all dots to inactive state */
    for (uint8_t i = 0; i < child_cnt; i++) {
        lv_obj_t *dot = lv_obj_get_child(indicator, i);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
    }
    
    /* Set active dot style */
    if (active_dot < child_cnt) {
        lv_obj_t *dot = lv_obj_get_child(indicator, active_dot);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    }
}

/* Gallery event callback (uses ONLY available Tileview APIs) */
static void gallery_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tileview = lv_event_get_target(e);
    lv_obj_t *indicator = lv_event_get_user_data(e);
    
    /* Only respond to VALUE_CHANGED (triggered by scroll end in your Tileview) */
    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_obj_update_layout(tileview);
        
        /* Calculate tile index using ONLY core LVGL APIs */
        int32_t tileview_width = lv_obj_get_content_width(tileview);
        lv_point_t scroll_end;
        lv_obj_get_scroll_end(tileview, &scroll_end);
        
        /* Calculate active tile index (matches your Tileview's internal logic) */
        uint8_t active_idx = (scroll_end.x + (tileview_width / 2)) / tileview_width;
        
        /* Update pagination indicator (使用重命名后的函数) */
        gallery_update_pagination_indicator(indicator, active_idx);
    }
}

/* Simplified button click handler (no msgbox to avoid API issues) */
static void gallery_btn_click_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;
    
    /* Toggle button opacity for click feedback */
    lv_obj_t *btn = lv_event_get_target(e);
    lv_opa_t current_opa = lv_obj_get_style_bg_opa(btn, 0);
    lv_obj_set_style_bg_opa(btn, (current_opa == LV_OPA_80) ? LV_OPA_100 : LV_OPA_80, 0);
}

/* Create gallery item (button as content) - 100% LVGL 9.3 compatible */
static void create_gallery_item(lv_obj_t *tile, const char *btn_text, lv_color_t btn_color)
{
    /* Create main gallery button (replaces image) */
    lv_obj_t *btn = lv_btn_create(tile);
    lv_obj_set_size(btn, lv_pct(80), lv_pct(70));
    lv_obj_center(btn);
    
    /* Style the button (gallery item appearance) */
    lv_obj_set_style_radius(btn, 16, 0);
    lv_obj_set_style_bg_color(btn, btn_color, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_80, 0);
    lv_obj_set_style_border_width(btn, 2, 0);
    
    /* Use universally available palette color */
    lv_obj_set_style_border_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_60, 0);
    
    /* Add button text */
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, btn_text);
    lv_obj_center(label);
    
    /* Use safe palette color */
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
    
    /* ========== 修复：字体参数加&取地址符，匹配lv_font_t*类型 ========== */
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    
    /* Add click event */
    lv_obj_add_event_cb(btn, gallery_btn_click_cb, LV_EVENT_CLICKED, NULL);
}

/* Main gallery creation function (ONLY uses your provided Tileview APIs) */
void create_lvgl_gallery(void)
{
    /* 1. Create Tileview (ONLY use lv_tileview_create from your API) */
    lv_obj_t *gallery = lv_tileview_create(lv_scr_act());
    lv_obj_set_size(gallery, lv_pct(100), lv_pct(100));
    lv_obj_set_scrollbar_mode(gallery, LV_SCROLLBAR_MODE_OFF);
    
    /* 2. Create gallery tiles (ONLY use lv_tileview_add_tile from your API) */
    uint8_t gallery_count = 5;
    lv_obj_t *tile[gallery_count];
    const char *btn_text_list[] = {
        "Gallery Item 1",
        "Gallery Item 2",
        "Gallery Item 3",
        "Gallery Item 4",
        "Gallery Item 5"
    };
    lv_color_t btn_color_list[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_ORANGE),
        lv_palette_main(LV_PALETTE_PURPLE)
    };
    
    /* Create horizontal tiles */
    for (uint8_t i = 0; i < gallery_count; i++) {
        tile[i] = lv_tileview_add_tile(gallery, i, 0, LV_DIR_HOR);
        create_gallery_item(tile[i], btn_text_list[i], btn_color_list[i]);
    }
    
    /* 3. Set initial tile (ONLY use lv_tileview_set_tile_by_index from your API) */
    lv_tileview_set_tile_by_index(gallery, 0, 0, LV_ANIM_ON);
    
    /* 4. Create pagination indicator (使用重命名后的函数) */
    lv_obj_t *indicator = gallery_create_pagination_indicator(lv_scr_act(), gallery_count);
    if (indicator) {
        lv_obj_align(indicator, LV_ALIGN_BOTTOM_MID, 0, -20);
        
        /* Add event listener */
        lv_obj_add_event_cb(gallery, gallery_event_cb, LV_EVENT_VALUE_CHANGED, indicator);
        
        /* Initialize indicator to first tile */
        gallery_update_pagination_indicator(indicator, 0);
    }
}

#endif /* LVGL_GALLERY_DEMO_H */
