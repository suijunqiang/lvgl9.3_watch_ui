#include "gui.h"
#include "../../../lvgl/lvgl.h"

static lv_obj_t *win1;
static lv_obj_t *win2;
#define ENABLE_WIN
static int s_switch = 0;


static void btn1_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if ( code == LV_EVENT_CLICKED)
    {
        printf("btn1 clicked\n");
        lv_obj_add_flag(win1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win2, LV_OBJ_FLAG_HIDDEN);
    }
}

static void btn2_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if ( code == LV_EVENT_CLICKED)
    {
        printf("btn2 clicked\n");
        lv_obj_add_flag(win2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win1, LV_OBJ_FLAG_HIDDEN);
    }

}

void my_gui(void)
{
    /* win1 */
    win1 = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(win1, lv_color_hex(0xeeff00), LV_STATE_DEFAULT);
    lv_obj_set_size(win1, LV_HOR_RES, LV_VER_RES);

    lv_obj_t *btn1 = lv_btn_create(win1);
    lv_obj_set_size(btn1, 100, 30);
    lv_obj_center(btn1);
    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "win1");
    lv_obj_center(label1);
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);

    /* win2 */
    win2 = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(win2, lv_color_hex(0x00d8db), LV_STATE_DEFAULT);
    lv_obj_set_size(win2, LV_HOR_RES, LV_VER_RES);

    lv_obj_t *btn2 = lv_btn_create(win2);
    lv_obj_set_size(btn2, 100, 30);
    lv_obj_center(btn2);
    lv_obj_t *label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "win2");
    lv_obj_center(label2);
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(win2, LV_OBJ_FLAG_HIDDEN);
}








