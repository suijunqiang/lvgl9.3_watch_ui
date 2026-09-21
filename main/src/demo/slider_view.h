/*
 * LVGL 9.3 Image Slider (Minimal Version for lv_port_pc_vscode)
 * Uses ONLY core LVGL 9.3 APIs (no missing style/ event APIs)
 * English comments + launcher_view.h style
 */
#include "../../../lvgl/lvgl.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

/* -------------------------- Configuration Parameters -------------------------- */
#define IMG_SLIDER_COUNT      5       // Total image slots (cycled with same image)
#define IMG_SLIDER_CENTER_X   160     // Screen horizontal center (320x240)
#define IMG_SLIDER_CENTER_Y   120     // Screen vertical center (320x240)
#define IMG_SLIDER_WIDTH      120     // Base image width
#define IMG_SLIDER_HEIGHT     80      // Base image height
#define IMG_SLIDER_SPACING    140     // Horizontal spacing between images
#define SCALE_MAX             1.0f    // Max scale for center image (100%)
#define SCALE_MIN             0.6f    // Min scale for side images (60%)
#define DRAG_THRESHOLD        50      // Threshold to trigger image switch

/* -------------------------- Global State (launcher_view.h style) -------------------------- */
typedef enum {
    VIEW_IMG_SLIDER_E,
    VIEW_IMG_SLIDER_COUNT_E
} view_id_img_slider;

// View registry (matches launcher_view.h pattern)
static struct {
    lv_obj_t *obj;
} img_slider_views[VIEW_IMG_SLIDER_COUNT_E] = {
    [VIEW_IMG_SLIDER_E] = NULL
};

// Core state (no z-index/antialias dependencies)
static lv_obj_t *img_container = NULL;
static lv_obj_t *img_objs[IMG_SLIDER_COUNT] = {NULL};
static int32_t current_img_idx = 0;
static int32_t drag_start_x = 0;
static lv_point_t drag_point;        // For input device position

/* LVGL 9.3 built-in test image (no external resources) */
LV_IMG_DECLARE(lv_img_test);

/* -------------------------- Function Prototypes -------------------------- */
static void update_image_positions(int32_t offset);
static void img_slider_event_handler(lv_event_t *e);
static void create_img_slider_interface(lv_obj_t *parent);
static void img_slider_switch_to(view_id_img_slider view_id);
void img_slider_launcher_init(void);
void lv_example_img_slider(void);

/* -------------------------- Core Implementation -------------------------- */
/**
 * Update position and scale of all slider images
 * Removes z-index/antialias (missing APIs in your LVGL port)
 * @param offset Temporary horizontal drag offset (pixels)
 */
static void update_image_positions(int32_t offset)
{
    for(int32_t i = 0; i < IMG_SLIDER_COUNT; i++) {
        if(img_objs[i] == NULL) continue;

        // Calculate horizontal position relative to center
        int32_t idx_diff = i - current_img_idx;
        int32_t target_x = IMG_SLIDER_CENTER_X + (idx_diff * IMG_SLIDER_SPACING) + offset;
        
        // Set base position (LVGL 9.3 core API - guaranteed to exist)
        lv_obj_set_x(img_objs[i], target_x);
        lv_obj_set_y(img_objs[i], IMG_SLIDER_CENTER_Y);

        // Calculate scale factor (distance-based scaling)
        float distance_ratio = (float)abs(target_x - IMG_SLIDER_CENTER_X) / IMG_SLIDER_SPACING;
        float scale = SCALE_MAX - ((SCALE_MAX - SCALE_MIN) * distance_ratio);
        scale = (scale < SCALE_MIN) ? SCALE_MIN : scale;

        // Apply transform scale (ONLY core LVGL 9.3 transform API)
        lv_obj_set_style_transform_scale_x(img_objs[i], scale, LV_PART_MAIN);
        lv_obj_set_style_transform_scale_y(img_objs[i], scale, LV_PART_MAIN);

        // Z-index REMOVED (missing lv_obj_set_style_z_index in your port)
        // Use layering via creation order (center image created last)
    }
}

/**
 * Event handler using ONLY existing LVGL events (no POINTER events)
 * Uses LV_EVENT_CLICKED + LV_EVENT_LONG_PRESSED_REPEAT for drag simulation
 * Fallback for missing pointer events in your LVGL port
 */
static void img_slider_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_indev_t *active_indev = lv_indev_active();
    
    if(active_indev == NULL) return;

    // Use MOUSE/TOUCH events available in ALL LVGL ports
    if(code == LV_EVENT_PRESSED) {
        // Record initial touch/mouse position (core API)
        lv_indev_get_point(active_indev, &drag_point);
        drag_start_x = drag_point.x;
    }
    else if(code == LV_EVENT_PRESSING) {
        // Update positions in real-time during press (simulate drag)
        lv_indev_get_point(active_indev, &drag_point);
        int32_t offset = drag_point.x - drag_start_x;
        update_image_positions(offset);
    }
    else if(code == LV_EVENT_RELEASED) {
        // Calculate drag distance and switch images
        lv_indev_get_point(active_indev, &drag_point);
        int32_t drag_delta = drag_point.x - drag_start_x;
        
        // Swipe left (next image)
        if(drag_delta < -DRAG_THRESHOLD) {
            current_img_idx = (current_img_idx + 1) % IMG_SLIDER_COUNT;
        }
        // Swipe right (previous image)
        else if(drag_delta > DRAG_THRESHOLD) {
            current_img_idx = (current_img_idx - 1 + IMG_SLIDER_COUNT) % IMG_SLIDER_COUNT;
        }

        // Snap back to center position
        update_image_positions(0);
    }
}

/**
 * Create image slider UI (NO missing APIs: z-index/antialias/drag removed)
 * Matches launcher_view.h create_*_interface pattern
 * @param parent Parent object (screen/container from launcher)
 */
static void create_img_slider_interface(lv_obj_t *parent)
{
    // 1. Create main container (core LVGL API)
    img_container = lv_obj_create(parent);
    lv_obj_set_size(img_container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(img_container, lv_color_hex(0x222222), LV_PART_MAIN);
    lv_obj_clear_flag(img_container, LV_OBJ_FLAG_SCROLLABLE);

    // 2. Create image objects (core LVGL image API)
    for(int32_t i = 0; i < IMG_SLIDER_COUNT; i++) {
        img_objs[i] = lv_img_create(img_container);
        lv_img_set_src(img_objs[i], &lv_img_test);                  // Built-in test image
        lv_obj_set_size(img_objs[i], IMG_SLIDER_WIDTH, IMG_SLIDER_HEIGHT); // Base size
        lv_obj_set_align(img_objs[i], LV_ALIGN_CENTER);             // Initial alignment

        // Antialias REMOVED (missing lv_obj_set_style_antialias in your port)
    }

    // 3. Configure events (ONLY existing events in your LVGL port)
    lv_obj_add_event_cb(img_container, img_slider_event_handler, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(img_container, img_slider_event_handler, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(img_container, img_slider_event_handler, LV_EVENT_RELEASED, NULL);

    // 4. Initialize image positions
    update_image_positions(0);
}

/**
 * Switch to slider view (matches launcher_switch_to pattern)
 * No z-index (uses core hide/show flags)
 * @param view_id Target view ID to activate
 */
static void img_slider_switch_to(view_id_img_slider view_id)
{
    // Hide all slider views
    for(int32_t i = 0; i < VIEW_IMG_SLIDER_COUNT_E; i++) {
        if(img_slider_views[i].obj != NULL) {
            lv_obj_add_flag(img_slider_views[i].obj, LV_OBJ_FLAG_HIDDEN);
        }
    }

    // Show target view (core LVGL API)
    if(img_slider_views[view_id].obj != NULL) {
        lv_obj_clear_flag(img_slider_views[view_id].obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(img_slider_views[view_id].obj); // Core layering API
    } else {
        printf("Warning: Image slider view %d not initialized\n", view_id);
    }
}

/**
 * Initialize slider (integrates with launcher_view.h)
 * No missing APIs - uses only core LVGL functions
 */
void img_slider_launcher_init(void)
{
    lv_obj_t *scr = lv_scr_act();

    // Register slider view (launcher pattern)
    img_slider_views[VIEW_IMG_SLIDER_E].obj = lv_obj_create(scr);
    lv_obj_set_size(img_slider_views[VIEW_IMG_SLIDER_E].obj, lv_pct(100), lv_pct(100));
    
    // Create slider interface
    create_img_slider_interface(img_slider_views[VIEW_IMG_SLIDER_E].obj);
    img_slider_switch_to(VIEW_IMG_SLIDER_E);
}

/**
 * Main entry point (matches main.c's lv_example_img_slider() call)
 * No SDL/extra APIs - fully compatible with lv_port_pc_vscode
 */
void lv_example_img_slider(void)
{
    // Initialize slider (integrates with launcher)
    img_slider_launcher_init();

    // Main loop (managed by lv_port_pc_vscode - do NOT add custom loop)
}
