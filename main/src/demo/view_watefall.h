#ifndef VIEW_WATERFALL_H
#define VIEW_WATERFALL_H

#include "../../../lvgl/lvgl.h"

#include "p3.c"
#include "p3_100_100.c"
#include "picture_1.c"
#include "picture_2.c"
#include "picture_3.c"
#include "picture_4.c"
#include "picture_5.c"
#include "picture_6.c"
#include "picture_7.c"
#include "picture_8.c"
#include "picture_9.c"
#include "picture_10.c"
#include "picture_11.c"
#include "picture_12.c"
#include "picture_13.c"
#include "picture_14.c"
#include "picture_15.c"
#include "picture_16.c"
#include "picture_17.c"
#include "picture_18.c"
#include "picture_19.c"


//图片数量，有几个图片写几个，即my_img长度
#define IMAGE_ICON_NUM  19
//每行几个图片
#define IMAGE_ROW_NUM   5
//图片尺寸
#define IMAGE_SIZE      80
//图片偏移（未用到）
#define IMAGE_OFFSET_X  0
#define IMAGE_OFFSET_Y  0
//图片缩放比例
#define IMAGE_SCALE     0.6f
#define IMAGE_ICON_OFFSET 400

//图片对象
lv_obj_t * image[IMAGE_ICON_NUM];
//初始坐标（程序自动根据以上参数计算）
lv_point_t init_pos[IMAGE_ICON_NUM];
//拖拽位置
lv_point_t point_final = {0,0};

LV_IMAGE_DECLARE(img_lvgl_logo);
LV_IMAGE_DECLARE(p3_100_100);
LV_IMAGE_DECLARE(img_demo_widgets_avatar);
LV_IMAGE_DECLARE(picture_1);
LV_IMAGE_DECLARE(picture_2);
LV_IMAGE_DECLARE(picture_3);
LV_IMAGE_DECLARE(picture_4);
LV_IMAGE_DECLARE(picture_5);
LV_IMAGE_DECLARE(picture_6);
LV_IMAGE_DECLARE(picture_7);
LV_IMAGE_DECLARE(picture_8);
LV_IMAGE_DECLARE(picture_9);
LV_IMAGE_DECLARE(picture_10);
LV_IMAGE_DECLARE(picture_11);
LV_IMAGE_DECLARE(picture_12);
LV_IMAGE_DECLARE(picture_13);
LV_IMAGE_DECLARE(picture_14);
LV_IMAGE_DECLARE(picture_15);
LV_IMAGE_DECLARE(picture_16);
LV_IMAGE_DECLARE(picture_17);
LV_IMAGE_DECLARE(picture_18);
LV_IMAGE_DECLARE(picture_19);

// 声明lv_img_dsc_t类型的数组my_img
const lv_img_dsc_t * my_img[] = {
    &picture_1,
    &picture_2,
    &picture_3,
    &picture_4,
    &picture_5,
    &picture_6,
    &picture_7,
    &picture_8,
    &picture_9,
    &picture_10,
    &picture_11,
    &picture_12,
    &picture_13,
    &picture_14,
    &picture_15,
    &picture_16,
    &picture_17,
    &picture_18,
    &picture_19,
};


// 独立的动画完成回调函数
static void anim_ready_cb(lv_anim_t * a) {
    lv_obj_t * obj = (lv_obj_t *)a->var;
    lv_obj_del(obj);
}


static void img_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        printf("图像被点击！\n");
        
        // 创建一个临时消息弹窗
        lv_obj_t * msg = lv_label_create(lv_scr_act());
        lv_label_set_text(msg, "Icon Clicked!");
        
        // 设置样式（可选）
        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_bg_color(&style, lv_color_hex(0x000000));
        lv_style_set_text_color(&style, lv_color_hex(0xFFFFFF));
        lv_style_set_pad_all(&style, 10);
        lv_obj_add_style(msg, &style, 0);
        
        // 居中显示
        lv_obj_center(msg);
        
        // 确保对象是可见的
        lv_obj_clear_flag(msg, LV_OBJ_FLAG_HIDDEN);
        
        // 3秒后自动删除消息
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_time(&a, 3000);
        lv_anim_set_ready_cb(&a, anim_ready_cb);
        lv_anim_set_var(&a, msg);
        lv_anim_start(&a);
    }
}

static void img_drag_event_handler(lv_event_t * e)
{
    int i;
    lv_sqrt_res_t lenth;
    int w = lv_obj_get_width(lv_screen_active());
    int h = lv_obj_get_height(lv_screen_active());
    lv_obj_t * obj = lv_event_get_target(e);
    lv_indev_t * indev = lv_indev_active();
    if(indev == NULL)  return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    point_final.x += vect.x;
    point_final.y += vect.y;

    for(i = 0;i < IMAGE_ICON_NUM;i++)
    {
        int pos_x,pos_y;
        pos_x = init_pos[i].x + point_final.x;
        pos_y = init_pos[i].y + point_final.y;
        //计算图片距离屏幕中心距
        lv_sqrt((pos_x - w / 2) * (pos_x - w / 2) + \
                (pos_y - h / 2) * (pos_y - h / 2), \
                &lenth, 0xFFFF);
        if(1)//根据距离屏幕中心距离确定缩放大小
        {
            //lv_obj_set_style_transform_scale(image[i],(400 - lenth.i) * IMAGE_SCALE,_LV_STYLE_STATE_CMP_SAME);
            lv_obj_set_style_transform_scale(image[i],(IMAGE_ICON_OFFSET - lenth.i) * IMAGE_SCALE, 0);
            lv_obj_set_pos(image[i], pos_x - (IMAGE_ICON_OFFSET - lenth.i) * 0.1, pos_y - (IMAGE_ICON_OFFSET - lenth.i) * 0.1);
        }
        else //一般大
        {
            lv_obj_set_pos(image[i], pos_x , pos_y);
        }
    }
}

void lv_demo_waterfall(void)
{
    int i;
    int offset_x,offset_y;
    lv_obj_t * parent = lv_screen_active();
    lv_obj_set_scrollbar_mode(parent,LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(parent,LV_DIR_NONE);

    offset_x = IMAGE_OFFSET_X;
    offset_y = IMAGE_OFFSET_Y;
    //初始化初始位置摆列
    for(i=0;i<IMAGE_ICON_NUM;i++)
    {
        int x,y,l,w,h;
        lv_sqrt_res_t len;
        w = lv_obj_get_width(parent);
        h = lv_obj_get_height(parent);


        image[i] = lv_image_create(parent);
        //lv_image_set_src(image[i], &img_lvgl_logo);
        //lv_image_set_src(image[i], &p3_70_70);
        lv_image_set_src(image[i], my_img[i]);
        //lv_obj_set_user_datag'gimage[i], (void *)(intptr_t)i);
        //image[i]->user_data = i;
        //lv_obj_set_user_data(image[i]->user_data, (void *)i);
        //lv_image_set_src(image[i],my_img[i]);
        if((i / IMAGE_ROW_NUM) % 2){
            x = (i % IMAGE_ROW_NUM) * IMAGE_SIZE + IMAGE_SIZE / 2 + offset_x;
        }
        else {
            x = (i % IMAGE_ROW_NUM) * IMAGE_SIZE + offset_x;
        }
        y = (i / IMAGE_ROW_NUM) * IMAGE_SIZE * 0.886f + offset_y;
        lv_sqrt((x - w / 2) * (x - w / 2) + (y - h / 2) * (y - h / 2), &len, 0xFFFF);
        if(1)
        {
            lv_obj_set_style_transform_scale(image[i],(w / 2 - len.i) * IMAGE_SCALE, 0);
        }
        else
        {
            lv_obj_set_style_transform_scale(image[i],128, 0);
        }
        lv_obj_set_x(image[i], x);
        lv_obj_set_y(image[i], y);
        init_pos[i].x = x;
        init_pos[i].y = y;
        lv_obj_add_flag(image[i],LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_add_flag(image[i],LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(image[i],img_drag_event_handler, LV_EVENT_PRESSING, NULL);
        lv_obj_add_event_cb(image[i], img_event_cb, LV_EVENT_ALL, NULL);

    }
}

#endif /* VIEW_WATERFALL_H */