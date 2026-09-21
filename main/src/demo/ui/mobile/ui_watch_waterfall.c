#include "ui_watch_waterfall.h"
#include <stdio.h>
#include <math.h>

// Keep counts similar to original waterfall demo
#define IMAGE_ICON_NUM  9
#define IMAGE_ROW_NUM   3
/* base icon size in pixels */
#define BASE_ICON_SIZE  48
/* scale multiplier for central emphasis (increase for more contrast) */
#define IMAGE_SCALE     1.2f
/* distance used for scaling falloff (smaller = tighter focus) */
#define IMAGE_ICON_OFFSET 200

static lv_obj_t *s_root = NULL;
static lv_obj_t *image_objs[IMAGE_ICON_NUM];
static lv_point_t init_pos[IMAGE_ICON_NUM];
static lv_point_t point_final = {0,0};

/* helper for drag tracking */
static lv_point_t last_point;
static bool last_point_valid = false;
static lv_timer_t * s_timer = NULL;

static void update_layout(void);

/* root size change handler so layout can be recalculated once the object
 * actually has a size.  This avoids the initial "w=0 h=0" problem where
 * update_layout is called during init before the display driver has set
 * the root dimensions.  We still call update_layout directly from init
 * and open, but the size changed event guarantees a later update when
 * the real size is available.
 */
static void root_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_SIZE_CHANGED) {
        update_layout();
    }
}

LV_IMAGE_DECLARE(img_lvgl_logo);

static void anim_ready_cb(lv_anim_t * a) {
    lv_obj_t * obj = (lv_obj_t *)a->var;
    lv_obj_del(obj);
}

static void img_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        printf("image clicked!\n");
        lv_obj_t * mbox1 = lv_msgbox_create(NULL);
        lv_msgbox_add_title(mbox1, "Hello");
        lv_msgbox_add_text(mbox1, "icon btn click.");
        lv_msgbox_add_close_button(mbox1);
    }
}

static void s_timer_cb(lv_timer_t * t)
{
    (void)t;
    if(!last_point_valid) return;
    lv_indev_t * indev = lv_indev_active();
    if(!indev) return;
    lv_point_t cur;
    lv_indev_get_point(indev, &cur);
    int dx = cur.x - last_point.x;
    int dy = cur.y - last_point.y;
    if(dx == 0 && dy == 0) return;
    last_point = cur;
    point_final.x += dx;
    point_final.y += dy;
    if (point_final.x > IMAGE_ICON_OFFSET) point_final.x = IMAGE_ICON_OFFSET;
    if (point_final.x < -IMAGE_ICON_OFFSET) point_final.x = -IMAGE_ICON_OFFSET;
    if (point_final.y > IMAGE_ICON_OFFSET) point_final.y = IMAGE_ICON_OFFSET;
    if (point_final.y < -IMAGE_ICON_OFFSET) point_final.y = -IMAGE_ICON_OFFSET;
    update_layout();
}

static void img_drag_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    printf("[wf] img_handler event=%d\n", (int)code);
    if(code != LV_EVENT_PRESSING) return;
    lv_indev_t * indev = lv_indev_active();
    if(!indev) return;
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    printf("[wf] pressing vect=(%d,%d)\n", vect.x, vect.y);
    point_final.x += vect.x;
    point_final.y += vect.y;
    if (point_final.x > IMAGE_ICON_OFFSET) point_final.x = IMAGE_ICON_OFFSET;
    if (point_final.x < -IMAGE_ICON_OFFSET) point_final.x = -IMAGE_ICON_OFFSET;
    if (point_final.y > IMAGE_ICON_OFFSET) point_final.y = IMAGE_ICON_OFFSET;
    if (point_final.y < -IMAGE_ICON_OFFSET) point_final.y = -IMAGE_ICON_OFFSET;
    printf("[wf] point_final=(%d,%d)\n", point_final.x, point_final.y);
    update_layout();
}

static void update_layout(void)
{
    if (!s_root) return;
    int w = lv_obj_get_width(s_root);
    int h = lv_obj_get_height(s_root);
    /* if we don't yet know the size just bail out; another update will
     * be triggered by the size changed event when the dimensions become
     * non-zero.  This also prevents the grid from initially being laid
     * out off-screen with bogus negative coordinates. */
    if(w == 0 || h == 0) {
        printf("[wf] update_layout skipped (w=%d h=%d)\n", w, h);
        return;
    }
    printf("[wf] update_layout w=%d h=%d\n", w, h);

    /* compute grid origin so the whole grid is centered */
    int cols = IMAGE_ROW_NUM;
    int rows = (IMAGE_ICON_NUM + cols - 1) / cols;
    int gap = 20;
    int total_w = cols * (BASE_ICON_SIZE + gap) - gap;
    int total_h = rows * (BASE_ICON_SIZE + gap) - gap;
    int start_x = w / 2 - total_w / 2;
    int start_y = h / 2 - total_h / 2;

    for (int i = 0; i < IMAGE_ICON_NUM; i++) {
        int col = i % cols;
        int row = i / cols;
        int base_x = start_x + col * (BASE_ICON_SIZE + gap) + BASE_ICON_SIZE / 2;
        int base_y = start_y + row * (BASE_ICON_SIZE + gap) + BASE_ICON_SIZE / 2;

        int pos_x = base_x + point_final.x;
        int pos_y = base_y + point_final.y;

        lv_sqrt_res_t lenth;
        int dx = pos_x - w / 2;
        int dy = pos_y - h / 2;
        uint32_t dist2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
        lv_sqrt(dist2, &lenth, 0xFFFF);

        /* compute a maximum distance (corner) for normalization */
        float maxd = sqrtf((w/2.0f)*(w/2.0f) + (h/2.0f)*(h/2.0f));
        float factor = 1.0f + IMAGE_SCALE * (1.0f - (float)lenth.i / maxd);
        if (factor < 0.5f) factor = 0.5f;
        if (factor > 2.0f) factor = 2.0f;
        lv_coord_t new_size = (lv_coord_t)(BASE_ICON_SIZE * factor);
        printf("[wf] layout i=%d pos=(%d,%d) dist=%d factor=%.2f size=%d\n",
               i, pos_x, pos_y, lenth.i, factor, (int)new_size);
        lv_obj_set_size(image_objs[i], new_size, new_size);
        lv_obj_set_pos(image_objs[i], pos_x - new_size / 2, pos_y - new_size / 2);
        if (factor > 1.2f) lv_obj_move_foreground(image_objs[i]);
    }
}


void ui_watch_waterfall_init(void)
{
    if (s_root) return;

    s_root = lv_obj_create(lv_scr_act());
    /* root not clickable; let child images handle input */
    /* Use display resolution to ensure width/height are available immediately */
    lv_coord_t disp_w = lv_disp_get_hor_res(NULL);
    lv_coord_t disp_h = lv_disp_get_ver_res(NULL);
    lv_obj_set_size(s_root, disp_w, disp_h);
    lv_obj_set_style_bg_color(s_root, lv_color_hex(0x202020), 0);
    lv_obj_set_pos(s_root, 0, 0);
    lv_obj_set_scrollbar_mode(s_root,LV_SCROLLBAR_MODE_OFF);

    /* watch for the root being sized by the layout system/driver so we can
     * position the icons correctly even if the initial size is not known
     * at this point. */
    lv_obj_add_event_cb(s_root, root_event_cb, LV_EVENT_SIZE_CHANGED, NULL);

    lv_obj_t * parent = s_root;
    int offset_x = 0, offset_y = 0;
    for(int i=0;i<IMAGE_ICON_NUM;i++)
    {
        int x,y,w,h;
        w = lv_obj_get_width(parent);
        h = lv_obj_get_height(parent);

        image_objs[i] = lv_image_create(parent);
        lv_image_set_src(image_objs[i], &img_lvgl_logo);

        /* layout in grid (row-major) using centers; start_x derived from
         * update_layout so here just approximate, will be corrected later */
        int col = i % IMAGE_ROW_NUM;
        int row = i / IMAGE_ROW_NUM;
        x = col * (BASE_ICON_SIZE + 20) + BASE_ICON_SIZE/2 + offset_x;
        y = row * (BASE_ICON_SIZE + 20) + BASE_ICON_SIZE/2 + offset_y;

        /* save center positions (unused now) */
        init_pos[i].x = x;
        init_pos[i].y = y;

        /* set initial size and position centered */
        lv_obj_set_size(image_objs[i], BASE_ICON_SIZE, BASE_ICON_SIZE);
        lv_obj_set_pos(image_objs[i], x - BASE_ICON_SIZE/2, y - BASE_ICON_SIZE/2);
        init_pos[i].x = x;
        init_pos[i].y = y;
        lv_obj_add_flag(image_objs[i],LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_flag(image_objs[i],LV_OBJ_FLAG_CLICKABLE);
        /* per-image press handling so dragging that starts on image works */
        lv_obj_add_event_cb(image_objs[i], img_drag_event_handler, LV_EVENT_PRESSING, NULL);
        lv_obj_add_event_cb(image_objs[i], img_event_cb, LV_EVENT_CLICKED, NULL);
        printf("[wf] init image %d\n", i);
    }

    /* allow dragging even when pressing outside icons */
    lv_obj_add_event_cb(s_root, img_drag_event_handler, LV_EVENT_PRESSING, NULL);

/* we no longer need a timer; dragging occurs on PRESSING via vect */

    /* initial layout attempt; if the root isn't sized yet this call will
     * be skipped and the size-changed callback above will handle it later. */
    update_layout();
}

void ui_watch_waterfall_open(void)
{
    if (s_root) lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
    /* make sure layout is correct when the view becomes visible */
    update_layout();
}

void ui_watch_waterfall_pause(void)
{
    if (s_root) lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_watch_waterfall_resume(void)
{
    if (s_root) lv_obj_clear_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_watch_waterfall_close(void)
{
    if (s_root) lv_obj_add_flag(s_root, LV_OBJ_FLAG_HIDDEN);
}

void ui_watch_waterfall_deinit(void)
{
    if (s_root) {
        lv_obj_del(s_root);
        s_root = NULL;
    }
}
