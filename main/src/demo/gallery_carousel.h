#ifndef LVGL_CONTINUOUS_GALLERY_H
#define LVGL_CONTINUOUS_GALLERY_H

#include "lvgl/lvgl.h"

/* Global gallery state */
static uint8_t total_count = 5;
static lv_obj_t *btns[5];
static const char *btn_texts[] = {"Btn 1", "Btn 2", "Btn 3", "Btn 4", "Btn 5"};
static lv_color_t btn_colors[5];
static lv_obj_t *gallery_container;
static uint8_t active_idx = 0; // Track the currently focused button

/*
 * Create pagination dot indicator
 */
static lv_obj_t *create_indicator(lv_obj_t *parent)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 80, 20);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    for(uint8_t i=0; i<total_count; i++) {
        lv_obj_t *dot = lv_obj_create(cont);
        lv_obj_remove_style_all(dot);
        lv_obj_set_size(dot, 6, 6);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
        if(i>0) lv_obj_set_style_margin_left(dot, 4, 0);
    }
    return cont;
}

/*
 * Update pagination indicator
 */
static void update_indicator(lv_obj_t *indicator, uint8_t active_idx)
{
    for(uint8_t i=0; i<total_count; i++) {
        lv_obj_t *dot = lv_obj_get_child(indicator, i);
        if(i == active_idx) {
            lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        } else {
            lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
            lv_obj_set_style_bg_opa(dot, LV_OPA_50, 0);
        }
    }
}

/*
 * Update button scaling and focus state
 */
static void update_button_state(void)
{
    lv_coord_t screen_center_x = lv_obj_get_width(gallery_container) / 2;
    lv_coord_t scroll_x = lv_obj_get_scroll_x(gallery_container);

    for(uint8_t i=0; i<total_count; i++) {
        lv_obj_t *btn = btns[i];
        lv_coord_t btn_x = lv_obj_get_x(btn) - scroll_x;
        lv_coord_t btn_center_x = btn_x + lv_obj_get_width(btn) / 2;
        lv_coord_t distance = abs(btn_center_x - screen_center_x);

        // Calculate scale (1.0 at center, 0.8 at edges)
        float scale = 1.0f - (float)distance / screen_center_x * 0.2f;
        if(scale < 0.8f) scale = 0.8f;

        lv_obj_set_style_transform_scale_x(btn, scale * 100, 0);
        lv_obj_set_style_transform_scale_y(btn, scale * 100, 0);

        // Set focus state (opacity + border color)
        if(i == active_idx) {
            lv_obj_set_style_opa(btn, LV_OPA_COVER, 0);
            lv_obj_set_style_border_color(btn, lv_color_white(), 0);
        } else {
            lv_obj_set_style_opa(btn, LV_OPA_80, 0);
            lv_obj_set_style_border_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
        }
    }

    // Update pagination indicator
    lv_obj_t *indicator = lv_obj_get_child(lv_scr_act(), 1);
    update_indicator(indicator, active_idx);
}

/*
 * Handle scroll events
 */
static void on_scroll(lv_event_t *e)
{
    if(lv_event_get_code(e) == LV_EVENT_SCROLL || lv_event_get_code(e) == LV_EVENT_SCROLL_END) {
        lv_coord_t screen_center_x = lv_obj_get_width(gallery_container) / 2;
        lv_coord_t scroll_x = lv_obj_get_scroll_x(gallery_container);

        // Find the button closest to center
        uint8_t new_active_idx = 0;
        lv_coord_t min_distance = LV_COORD_MAX;
        for(uint8_t i=0; i<total_count; i++) {
            lv_obj_t *btn = btns[i];
            lv_coord_t btn_x = lv_obj_get_x(btn);
            lv_coord_t btn_center_x = btn_x + lv_obj_get_width(btn) / 2;
            lv_coord_t distance = abs(btn_center_x - (scroll_x + screen_center_x));
            if(distance < min_distance) {
                min_distance = distance;
                new_active_idx = i;
            }
        }
        active_idx = new_active_idx;

        // Auto-center the active button (核心修复：精准计算目标位置)
        if(lv_event_get_code(e) == LV_EVENT_SCROLL_END) {
            lv_obj_t *active_btn = btns[active_idx];
            // 计算按钮中心到容器左边缘的距离
            lv_coord_t btn_abs_center_x = lv_obj_get_x(active_btn) + lv_obj_get_width(active_btn) / 2;
            // 目标滚动位置 = 按钮中心x - 屏幕中心x（确保按钮居中）
            lv_coord_t target_x = btn_abs_center_x - screen_center_x;
            
            // 强制滚动到目标位置
            lv_obj_scroll_to(gallery_container, target_x, 0, LV_ANIM_ON);
        }

        update_button_state();
    }
}

/*
 * Create a single gallery button
 */
static lv_obj_t *create_btn(lv_obj_t *parent, const char *text, lv_color_t color)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_remove_style_all(btn);
    // Larger button size
    lv_obj_set_size(btn, 200, 280);
    lv_obj_set_style_radius(btn, 16, 0);
    lv_obj_set_style_bg_color(btn, color, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_80, 0);
    lv_obj_set_style_border_width(btn, 3, 0);
    lv_obj_set_style_border_color(btn, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_70, 0);
    lv_obj_set_style_transform_pivot_x(btn, lv_pct(50), 0);
    lv_obj_set_style_transform_pivot_y(btn, lv_pct(50), 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);

    return btn;
}

/*
 * Create continuous scroll gallery
 */
void create_continuous_gallery(void)
{
    // Initialize colors
    btn_colors[0] = lv_palette_main(LV_PALETTE_RED);
    btn_colors[1] = lv_palette_main(LV_PALETTE_GREEN);
    btn_colors[2] = lv_palette_main(LV_PALETTE_BLUE);
    btn_colors[3] = lv_palette_main(LV_PALETTE_ORANGE);
    btn_colors[4] = lv_palette_main(LV_PALETTE_PURPLE);

    // Create main container
    gallery_container = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(gallery_container);
    lv_obj_set_size(gallery_container, 480, 320);
    lv_color_t black = lv_color_hex(0x000000);
    lv_obj_set_style_bg_color(gallery_container, black, 0);
    lv_obj_set_style_bg_opa(gallery_container, LV_OPA_COVER, 0);
    lv_obj_set_scroll_dir(gallery_container, LV_DIR_HOR);
    lv_obj_set_scrollbar_mode(gallery_container, LV_SCROLLBAR_MODE_OFF);

    // 核心修复：重新计算按钮排列位置，确保第五个能滚动到中心
    lv_coord_t btn_width = 200;
    lv_coord_t btn_spacing = 40;
    lv_coord_t screen_center_x = 240;
    // 第一个按钮的x坐标 = 屏幕中心 - 按钮宽度/2（初始就居中）
    lv_coord_t first_btn_x = screen_center_x - btn_width / 2;
    
    for(uint8_t i=0; i<total_count; i++) {
        btns[i] = create_btn(gallery_container, btn_texts[i], btn_colors[i]);
        // 每个按钮的x坐标 = 第一个按钮x + i*(按钮宽度+间距)
        lv_obj_set_pos(btns[i], first_btn_x + i*(btn_width + btn_spacing), (320 - 280)/2);
    }

    // Create pagination indicator
    lv_obj_t *indicator = create_indicator(lv_scr_act());

    // Add event listener
    lv_obj_add_event_cb(gallery_container, on_scroll, LV_EVENT_SCROLL, NULL);
    lv_obj_add_event_cb(gallery_container, on_scroll, LV_EVENT_SCROLL_END, NULL);

    // Initial state
    update_button_state();
}

#endif /* LVGL_CONTINUOUS_GALLERY_H */
