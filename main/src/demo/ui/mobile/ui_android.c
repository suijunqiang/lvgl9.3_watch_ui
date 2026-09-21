#include "ui_android.h"
#include <stdio.h>

// Optimized dimensions for 320x480 (fix bottom cut-off)
#define SCREEN_WIDTH          320
#define SCREEN_HEIGHT         480
#define STATUS_BAR_H          32
#define NAV_BAR_H             45  // Reduced height to fit screen
#define INDICATOR_H           12  // Reduced indicator height
#define APP_ICON_SIZE         55
#define APP_ICON_GAP          6

// Forward declarations
static void create_page(lv_obj_t *tile, int page_idx);
static void tile_change_cb(lv_event_t *e);

// Global objects
static lv_obj_t *s_root = NULL;
static lv_obj_t *s_tileview = NULL;
static lv_obj_t *s_indicators[3] = {NULL};
static lv_obj_t *s_tiles[3] = {NULL};

/**
 * @brief  Initialize Android launcher (320x480, fix bottom cut-off)
 */
void ui_android_init(void)
{
    if (s_root) return;

    // 1. Root container (320x480, WHITE background)
    s_root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(s_root, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(s_root, lv_color_white(), 0); 
    lv_obj_set_style_pad_all(s_root, 0, 0);
    lv_obj_align(s_root, LV_ALIGN_TOP_LEFT, 0, 0); // Critical: Align to top-left (not center)
    lv_obj_set_scrollbar_mode(s_root, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_clip_corner(s_root, false, 0);

    // 2. Status bar (TOP - WHITE background with black text)
    lv_obj_t *status_bar = lv_obj_create(s_root);
    lv_obj_set_size(status_bar, SCREEN_WIDTH, STATUS_BAR_H);
    lv_obj_set_pos(status_bar, 0, 0); // Align to top-left
    lv_obj_set_style_bg_color(status_bar, lv_color_white(), 0); 
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_scrollbar_mode(status_bar, LV_SCROLLBAR_MODE_OFF);

    // Status bar text (BLACK text on white background)
    lv_obj_t *time = lv_label_create(status_bar);
    lv_label_set_text(time, "10:24");
    lv_obj_set_style_text_color(time, lv_color_black(), 0);
    lv_obj_align(time, LV_ALIGN_LEFT_MID, 8, 0);

    lv_obj_t *wifi = lv_label_create(status_bar);
    lv_label_set_text(wifi, "WiFi");
    lv_obj_set_style_text_color(wifi, lv_color_black(), 0);
    lv_obj_align(wifi, LV_ALIGN_RIGHT_MID, -40, 0);

    lv_obj_t *batt = lv_label_create(status_bar);
    lv_label_set_text(batt, "85%");
    lv_obj_set_style_text_color(batt, lv_color_black(), 0);
    lv_obj_align(batt, LV_ALIGN_RIGHT_MID, -8, 0);

    // 3. Swipeable icon pages (core area - fit remaining space)
    s_tileview = lv_tileview_create(s_root);
    // Calculate tileview height to fit perfectly (no overflow)
    int tileview_height = SCREEN_HEIGHT - STATUS_BAR_H - NAV_BAR_H - INDICATOR_H;
    lv_obj_set_size(s_tileview, SCREEN_WIDTH, tileview_height);
    lv_obj_set_pos(s_tileview, 0, STATUS_BAR_H); // Align below status bar
    lv_obj_set_style_bg_color(s_tileview, lv_color_white(), 0); 
    lv_obj_set_style_border_width(s_tileview, 0, 0);
    lv_obj_set_scrollbar_mode(s_tileview, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(s_tileview, tile_change_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Create 3 horizontally swipeable pages
    s_tiles[0] = lv_tileview_add_tile(s_tileview, 0, 0, LV_DIR_HOR);
    create_page(s_tiles[0], 1);

    s_tiles[1] = lv_tileview_add_tile(s_tileview, 1, 0, LV_DIR_HOR);
    create_page(s_tiles[1], 2);

    s_tiles[2] = lv_tileview_add_tile(s_tileview, 2, 0, LV_DIR_HOR);
    create_page(s_tiles[2], 3);

    // 4. Page indicators (above nav bar - position correctly)
    lv_obj_t *ind_panel = lv_obj_create(s_root);
    lv_obj_set_size(ind_panel, SCREEN_WIDTH, INDICATOR_H);
    // Position indicator panel just above nav bar (no overlap)
    lv_obj_set_pos(ind_panel, 0, SCREEN_HEIGHT - NAV_BAR_H - INDICATOR_H);
    lv_obj_set_style_bg_color(ind_panel, lv_color_white(), 0); 
    lv_obj_set_style_border_width(ind_panel, 0, 0);
    lv_obj_set_scrollbar_mode(ind_panel, LV_SCROLLBAR_MODE_OFF);

    // 3 indicator dots (black/active, light gray/inactive)
    for (int i = 0; i < 3; i++) {
        s_indicators[i] = lv_obj_create(ind_panel);
        lv_obj_set_size(s_indicators[i], 6, 6);
        lv_obj_align(s_indicators[i], LV_ALIGN_CENTER, -12 + 12*i, 0);
        lv_obj_set_style_radius(s_indicators[i], 3, 0);
        lv_obj_set_style_bg_color(s_indicators[i], (i == 0) ? lv_color_black() : lv_color_hex(0xCCCCCC), 0);
    }

    // 5. Bottom navigation bar (FULLY VISIBLE - no cut-off)
    lv_obj_t *nav_bar = lv_obj_create(s_root);
    lv_obj_set_size(nav_bar, SCREEN_WIDTH, NAV_BAR_H);
    // Critical: Position nav bar at bottom (no overflow)
    lv_obj_set_pos(nav_bar, 0, SCREEN_HEIGHT - NAV_BAR_H);
    lv_obj_set_style_bg_color(nav_bar, lv_color_white(), 0); 
    lv_obj_set_style_border_width(nav_bar, 0, 0);
    lv_obj_set_scrollbar_mode(nav_bar, LV_SCROLLBAR_MODE_OFF);

    // 4 navigation buttons (FULLY VISIBLE, optimized size/position)
    const char *labels[] = {"Phone", "Msg", "Home", "Browser"};
    // Adjusted positions + smaller buttons to fit 320px width fully
    int btn_x[] = {8, 86, 164, 242}; 
    int btn_width = 58;  // Narrower buttons to fit text
    int btn_height = 32; // Shorter buttons to fit nav bar

    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = lv_btn_create(nav_bar);
        lv_obj_set_size(btn, btn_width, btn_height);
        lv_obj_set_pos(btn, btn_x[i], 6); // Centered vertically in nav bar
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x2196F3), 0); 
        lv_obj_set_style_radius(btn, 6, 0);

        lv_obj_t *lab = lv_label_create(btn);
        lv_label_set_text(lab, labels[i]);
        lv_obj_set_style_text_color(lab, lv_color_white(), 0);
        //lv_obj_set_style_text_font(lab, &lv_font_montserrat_10, 0); // Smaller font for full text
        lv_obj_center(lab);
    }
}

/**
 * @brief  Create single page with 10 app icons (320x480 optimized)
 * @param  tile: Parent tile object
 * @param  page_idx: Page number (1-3)
 */
static void create_page(lv_obj_t *tile, int page_idx)
{
    // Android-style app icon colors (vibrant on white background)
    const lv_color_t colors[] = {
        lv_color_hex(0xFF5252), lv_color_hex(0x4CAF50), lv_color_hex(0x2196F3),
        lv_color_hex(0xFFC107), lv_color_hex(0x9C27B0), lv_color_hex(0x00BCD4),
        lv_color_hex(0xFF9800), lv_color_hex(0x8BC34A), lv_color_hex(0xE91E63),
        lv_color_hex(0x607D8B)
    };

    // Recalculated icon positions (fit within tileview)
    int icon_positions[10][2] = {
        {15, 10},  {115, 10},  {215, 10},
        {15, 80},  {115, 80},  {215, 80},
        {15, 150}, {115, 150}, {215, 150},
        {115, 220}
    };

    // Create 10 fully visible app icons
    for (int i = 0; i < 10; i++) {
        lv_obj_t *icon = lv_btn_create(tile);
        lv_obj_set_size(icon, APP_ICON_SIZE, APP_ICON_SIZE);
        lv_obj_set_pos(icon, icon_positions[i][0], icon_positions[i][1]);
        lv_obj_set_style_bg_color(icon, colors[i], 0);
        lv_obj_set_style_radius(icon, 10, 0);
        lv_obj_set_scrollbar_mode(icon, LV_SCROLLBAR_MODE_OFF);

        // Icon text (white text on colored icons)
        char label[16];
        snprintf(label, sizeof(label), "A%d-%d", page_idx, i+1);
        lv_obj_t *lab = lv_label_create(icon);
        lv_label_set_text(lab, label);
        lv_obj_set_style_text_color(lab, lv_color_white(), 0);
        lv_obj_center(lab);
    }
}

/**
 * @brief  Update page indicators when swiping between pages
 * @param  e: LVGL event object
 */
static void tile_change_cb(lv_event_t *e)
{
    lv_obj_t *active_tile = lv_tileview_get_tile_act(s_tileview);
    for (int i = 0; i < 3; i++) {
        lv_obj_set_style_bg_color(s_indicators[i],
            (active_tile == s_tiles[i]) ? lv_color_black() : lv_color_hex(0xCCCCCC), 0);
    }
}

// Required API functions
void ui_android_open(void)
{
    if (s_root) lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_android_pause(void)
{
    if (s_root) lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_android_resume(void)
{
    if (s_root) lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_android_close(void)
{
    if (s_root) lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_android_deinit(void)
{
    if (s_root) {
        lv_obj_del(s_root);
        s_root = NULL;
    }
}