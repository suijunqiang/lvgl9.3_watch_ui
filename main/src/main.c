
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

#include "demo/p3.c"
#include "demo/p3_100_100.c"
#include "demo/picture_1.c"
#include "demo/picture_2.c"
#include "demo/picture_3.c"
#include "demo/picture_4.c"
#include "demo/picture_5.c"
#include "demo/picture_6.c"
#include "demo/picture_7.c"
#include "demo/picture_8.c"
#include "demo/picture_9.c"
#include "demo/picture_10.c"
#include "demo/picture_11.c"
#include "demo/picture_12.c"
#include "demo/picture_13.c"
#include "demo/picture_14.c"
#include "demo/picture_15.c"
#include "demo/picture_16.c"
#include "demo/picture_17.c"
#include "demo/picture_18.c"
#include "demo/picture_19.c"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

extern void freertos_main(void);

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if 1
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
#define IMAGE_SCALE     0.5f

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
            lv_obj_set_style_transform_scale(image[i],(400 - lenth.i) * IMAGE_SCALE, 0);
            lv_obj_set_pos(image[i], pos_x - (400 - lenth.i) * 0.1, pos_y - (400 - lenth.i) * 0.1);
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
    }
}
#endif


int SDL_main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  //hal_init(320, 480);
  hal_init(320, 320);

  #if LV_USE_OS == LV_OS_NONE

  /* Run the default demo */
  /* To try a different demo or example, replace this with one of: */
  /* - lv_demo_benchmark(); */
  /* - lv_demo_stress(); */
  /* - lv_example_label_1(); */
  /* - etc. */
  //lv_demo_widgets();
  //lv_demo_music();
  //lv_demos_show_help();
  //lv_demo_stress();
  lv_demo_waterfall();

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
#ifdef _MSC_VER
    Sleep(5);
#else
    usleep(5 * 1000);
#endif
  }

  #elif LV_USE_OS == LV_OS_FREERTOS

  /* Run FreeRTOS and create lvgl task */
  freertos_main();

  #endif

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
