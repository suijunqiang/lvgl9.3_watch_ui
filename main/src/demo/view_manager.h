#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include "../../../lvgl/lvgl.h"

/* 定义视图ID */
typedef enum {
    VIEW_MAIN,
    VIEW_SETTINGS,
    VIEW_MUSIC,
    VIEW_GALLERY,
    VIEW_COUNT
} view_id_t;

/* 视图回调函数类型定义 */
typedef void (*view_create_cb_t)(lv_obj_t * parent);
typedef void (*view_destroy_cb_t)(void);

/* 当前活动视图 */
static view_id_t current_view = VIEW_MAIN;

/* 主视图对象 */
static lv_obj_t * main_view = NULL;

/* 视图管理器初始化 */
void view_manager_init(void);

/* 视图创建函数 */
static void create_main_view(lv_obj_t * parent);
static void create_settings_view(lv_obj_t * parent);
static void create_music_view(lv_obj_t * parent);
static void create_gallery_view(lv_obj_t * parent);

/* 视图销毁函数 */
static void destroy_main_view(void);
static void destroy_settings_view(void);
static void destroy_music_view(void);
static void destroy_gallery_view(void);

/* 视图切换函数 */
static void switch_to_view(view_id_t view_id);
static void return_to_main_view(lv_event_t * e);

/* 图标点击事件处理函数 */
static void icon_click_event_handler(lv_event_t * e);

/* 视图注册表 */
static struct {
    view_create_cb_t create_cb;
    view_destroy_cb_t destroy_cb;
    lv_obj_t * obj;
} views[VIEW_COUNT] = {
    [VIEW_MAIN] = {create_main_view, destroy_main_view, NULL},
    [VIEW_SETTINGS] = {create_settings_view, destroy_settings_view, NULL},
    [VIEW_MUSIC] = {create_music_view, destroy_music_view, NULL},
    [VIEW_GALLERY] = {create_gallery_view, destroy_gallery_view, NULL}
};

/* 主函数 */
void lv_example_view_manager(void)
{
    view_manager_init();
}

/* 视图管理器初始化 */
void view_manager_init(void)
{
    /* 创建主视图 */
    switch_to_view(VIEW_MAIN);
}

/* 切换到指定视图 */
static void switch_to_view(view_id_t view_id)
{
     printf("switch_to_view \n");
    /* 销毁当前视图（如果不是主视图） */
    if(current_view != VIEW_MAIN && views[current_view].destroy_cb) {
        printf("in current_view if\n");
        views[current_view].destroy_cb();
    }

    /* 更新当前视图ID */
    current_view = view_id;

    /* 如果目标视图不存在，则创建它 */
    if(!views[view_id].obj && views[view_id].create_cb) {
        printf("in views create_cb if\n");
        lv_obj_t * parent = lv_scr_act();
        views[view_id].create_cb(parent);
    }

    /* 显示目标视图 */
    if(views[view_id].obj) {
        printf("in lv_obj_move_foreground if\n");
        lv_obj_clear_flag(views[view_id].obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(views[view_id].obj);
    } else {
        printf("Error: View object is NULL for view_id: %d\n", view_id);
    }
}

/* 返回主视图的回调函数 */
static void return_to_main_view(lv_event_t * e)
{
    switch_to_view(VIEW_MAIN);
}

/* 图标点击事件处理函数 */
static void icon_click_event_handler(lv_event_t * e)
{
     printf("icon_click_event_handler\n");
    view_id_t view_id = (view_id_t)lv_event_get_user_data(e);
    switch_to_view(view_id);
}

/* 创建主视图 */
static void create_main_view(lv_obj_t * parent)
{
    /* 创建主视图容器 */
    main_view = lv_obj_create(parent);
    lv_obj_set_size(main_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_view, LV_OBJ_FLAG_SCROLLABLE);

    /* 设置主视图样式 */
    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_color(&style_main, lv_color_hex(0x202020));
    lv_obj_add_style(main_view, &style_main, 0);

    /* 创建标题 */
    lv_obj_t * title = lv_label_create(main_view);
    lv_label_set_text(title, "Main Menu");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* 创建图标网格 */
    lv_obj_t * icon_grid = lv_obj_create(main_view);
    lv_obj_set_size(icon_grid, LV_PCT(90), LV_PCT(70));
    lv_obj_align(icon_grid, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(icon_grid, LV_OBJ_FLAG_SCROLLABLE);

    /* 设置网格布局 */
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(icon_grid, col_dsc, row_dsc);

    /* 定义图标和标签 */
    const char * icons[] = {
        LV_SYMBOL_SETTINGS,
        LV_SYMBOL_PLAY,
        LV_SYMBOL_IMAGE
    };

    const char * labels[] = {
        "Settings",
        "Music",
        "Gallery"
    };

    view_id_t view_ids[] = {
        VIEW_SETTINGS,
        VIEW_MUSIC,
        VIEW_GALLERY
    };

    /* 创建图标按钮 */
    for(int i = 0; i < 3; i++) {
        int row = i / 3;
        int col = i % 3;

        /* 创建按钮 */
        lv_obj_t * btn = lv_btn_create(icon_grid);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1,
                              LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(btn, 100, 100);

        /* 设置按钮样式 */
        static lv_style_t style_btn;
        lv_style_init(&style_btn);
        lv_style_set_bg_color(&style_btn, lv_color_hex(0x303030));
        lv_style_set_bg_opa(&style_btn, LV_OPA_80);
        lv_style_set_radius(&style_btn, 20);
        lv_style_set_border_width(&style_btn, 2);
        lv_style_set_border_color(&style_btn, lv_color_hex(0x505050));
        lv_obj_add_style(btn, &style_btn, 0);

        /* 按钮按下样式 */
        static lv_style_t style_btn_pr;
        lv_style_init(&style_btn_pr);
        lv_style_set_bg_color(&style_btn_pr, lv_color_hex(0x404040));
        lv_obj_add_style(btn, &style_btn_pr, LV_STATE_PRESSED);

        /* 添加图标 */
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "%s\n%s", icons[i], labels[i]);
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

        /* 设置点击事件 */
        lv_obj_add_event_cb(btn, icon_click_event_handler, LV_EVENT_CLICKED, (void*)view_ids[i]);
    }

    views[VIEW_MAIN].obj = main_view;

}

/* 销毁主视图 */
static void destroy_main_view(void)
{
    if(main_view) {
        lv_obj_add_flag(main_view, LV_OBJ_FLAG_HIDDEN);
         main_view = NULL;
        views[VIEW_MAIN].obj = NULL;
        // 主视图不销毁，保持状态
    }
}

/* 创建设置视图 */
static void create_settings_view(lv_obj_t * parent)
{
    /* 创建设置视图容器 */
    lv_obj_t * settings_view = lv_obj_create(parent);
    lv_obj_set_size(settings_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(settings_view, LV_OBJ_FLAG_SCROLLABLE);

    /* 设置样式 */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x1E1E1E));
    lv_obj_add_style(settings_view, &style, 0);

    /* 返回按钮 */
    lv_obj_t * back_btn = lv_btn_create(settings_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* 标题 */
    lv_obj_t * title = lv_label_create(settings_view);
    lv_label_set_text(title, "Settings");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* 创建TileView */
    lv_obj_t * tileview = lv_tileview_create(settings_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* 设置滚动方向和缩放级别（兼容LVGL 9.3） */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100%缩放
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* 创建3个Tile */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* 第一个Tile: System Settings */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile1, LV_OBJ_FLAG_SCROLLABLE);

    /* 添加设置项 */
    lv_obj_t * label1 = lv_label_create(tile1);
    lv_label_set_text(label1, "System Settings");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_18, 0);
    lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 20);

    /* 第二个Tile: Network Settings */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * label2 = lv_label_create(tile2);
    lv_label_set_text(label2, "Network Settings");
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_18, 0);
    lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 20);

    /* 第三个Tile: About */
    lv_obj_t * tile3 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile3, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_clear_flag(tile3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * label3 = lv_label_create(tile3);
    lv_label_set_text(label3, "About");
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_18, 0);
    lv_obj_align(label3, LV_ALIGN_TOP_MID, 0, 20);

    /* 设置初始Tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);

    /* 保存视图对象 */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS].obj = settings_view;
}

/* 销毁设置视图 */
static void destroy_settings_view(void)
{
    lv_obj_t * settings_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS].obj;
    if(settings_view) {
        lv_obj_del(settings_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_SETTINGS].obj = NULL;
    }
}

/* 创建音乐视图 */
static void create_music_view(lv_obj_t * parent)
{
    /* 创建音乐视图容器 */
    lv_obj_t * music_view = lv_obj_create(parent);
    lv_obj_set_size(music_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(music_view, LV_OBJ_FLAG_SCROLLABLE);

    /* 设置样式 */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x1A2456));
    lv_obj_add_style(music_view, &style, 0);

    /* 返回按钮 */
    lv_obj_t * back_btn = lv_btn_create(music_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* 标题 */
    lv_obj_t * title = lv_label_create(music_view);
    lv_label_set_text(title, "Music Player");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* 创建TileView */
    lv_obj_t * tileview = lv_tileview_create(music_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* 设置滚动方向和缩放级别（兼容LVGL 9.3） */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100%缩放
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* 创建2个Tile */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* 第一个Tile: Playlist */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* 添加播放列表 */
    lv_obj_t * list = lv_list_create(tile1);
    lv_obj_set_size(list, LV_PCT(90), LV_PCT(90));
    lv_obj_center(list);

    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 1");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 2");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 3");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 4");
    lv_list_add_btn(list, LV_SYMBOL_AUDIO, "Song 5");

    /* 第二个Tile: Play controls */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* 专辑封面 */
    lv_obj_t * album_cover = lv_obj_create(tile2);
    lv_obj_set_size(album_cover, 150, 150);
    lv_obj_align(album_cover, LV_ALIGN_TOP_MID, 0, 20);

    static lv_style_t style_cover;
    lv_style_init(&style_cover);
    lv_style_set_bg_color(&style_cover, lv_color_hex(0x3A4476));
    lv_style_set_radius(&style_cover, LV_RADIUS_CIRCLE);
    lv_obj_add_style(album_cover, &style_cover, 0);

    /* 播放控制按钮 */
    lv_obj_t * btn_prev = lv_btn_create(tile2);
    lv_obj_set_size(btn_prev, 40, 40);
    lv_obj_align(btn_prev, LV_ALIGN_BOTTOM_LEFT, 40, -40);
    lv_obj_t * label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, LV_SYMBOL_PREV);
    lv_obj_center(label_prev);

    lv_obj_t * btn_play = lv_btn_create(tile2);
    lv_obj_set_size(btn_play, 60, 60);
    lv_obj_align(btn_play, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_t * label_play = lv_label_create(btn_play);
    lv_label_set_text(label_play, LV_SYMBOL_PLAY);
    lv_obj_center(label_play);

    lv_obj_t * btn_next = lv_btn_create(tile2);
    lv_obj_set_size(btn_next, 40, 40);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -40, -40);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, LV_SYMBOL_NEXT);
    lv_obj_center(label_next);

    /* 设置初始Tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);

    /* 保存视图对象 */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC].obj = music_view;
}

/* 销毁音乐视图 */
static void destroy_music_view(void)
{
    lv_obj_t * music_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC].obj;
    if(music_view) {
        lv_obj_del(music_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_MUSIC].obj = NULL;
    }
}

/* 创建图库视图 */
static void create_gallery_view(lv_obj_t * parent)
{
    /* 创建图库视图容器 */
    lv_obj_t * gallery_view = lv_obj_create(parent);
    lv_obj_set_size(gallery_view, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(gallery_view, LV_OBJ_FLAG_SCROLLABLE);

    /* 设置样式 */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_hex(0x352F44));
    lv_obj_add_style(gallery_view, &style, 0);

    /* 返回按钮 */
    lv_obj_t * back_btn = lv_btn_create(gallery_view);
    lv_obj_set_size(back_btn, 60, 40);
    lv_obj_align(back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(back_btn, return_to_main_view, LV_EVENT_CLICKED, NULL);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT);
    lv_obj_center(back_label);

    /* 标题 */
    lv_obj_t * title = lv_label_create(gallery_view);
    lv_label_set_text(title, "Gallery");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    /* 创建TileView */
    lv_obj_t * tileview = lv_tileview_create(gallery_view);
    lv_obj_set_size(tileview, LV_PCT(100), LV_PCT(85));
    lv_obj_align(tileview, LV_ALIGN_BOTTOM_MID, 0, 0);

    /* 设置滚动方向和缩放级别（兼容LVGL 9.3） */
    lv_obj_set_style_size(tileview, 100, 10, 0); // 100%缩放
    lv_obj_add_flag(tileview, LV_OBJ_FLAG_SCROLL_ELASTIC);

    /* 创建2个Tile */
    static lv_coord_t tile_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t tile_row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tileview, tile_col_dsc, tile_row_dsc);

    /* 第一个Tile: Thumbnails */
    lv_obj_t * tile1 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* 创建图像网格 */
    lv_obj_t * img_grid = lv_obj_create(tile1);
    lv_obj_set_size(img_grid, LV_PCT(90), LV_PCT(90));
    lv_obj_center(img_grid);

    static lv_coord_t grid_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t grid_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(img_grid, grid_col_dsc, grid_row_dsc);

    /* 添加示例图像 */
    for(int i = 0; i < 6; i++) {
        int row = i / 2;
        int col = i % 2;

        lv_obj_t * img_cont = lv_obj_create(img_grid);
        lv_obj_set_grid_cell(img_cont, LV_GRID_ALIGN_CENTER, col, 1,
                            LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(img_cont, 120, 80);

        static lv_style_t style_img;
        lv_style_init(&style_img);
        lv_style_set_bg_color(&style_img, lv_color_hex(0x5C5470));
        lv_style_set_radius(&style_img, 10);
        lv_obj_add_style(img_cont, &style_img, 0);

        lv_obj_t * img_label = lv_label_create(img_cont);
        lv_label_set_text_fmt(img_label, "Image %d", i + 1);
        lv_obj_center(img_label);
    }

    /* 第二个Tile: Image viewer */
    lv_obj_t * tile2 = lv_obj_create(tileview);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    /* 大图像显示区域 */
    lv_obj_t * img_view = lv_obj_create(tile2);
    lv_obj_set_size(img_view, LV_PCT(90), LV_PCT(70));
    lv_obj_align(img_view, LV_ALIGN_TOP_MID, 0, 20);

    static lv_style_t style_img_view;
    lv_style_init(&style_img_view);
    lv_style_set_bg_color(&style_img_view, lv_color_hex(0x5C5470));
    lv_style_set_radius(&style_img_view, 10);
    lv_obj_add_style(img_view, &style_img_view, 0);

    lv_obj_t * img_view_label = lv_label_create(img_view);
    lv_label_set_text(img_view_label, "Image Preview");
    lv_obj_center(img_view_label);

    /* 图像控制按钮 */
    lv_obj_t * btn_prev = lv_btn_create(tile2);
    lv_obj_set_size(btn_prev, 40, 40);
    lv_obj_align(btn_prev, LV_ALIGN_BOTTOM_LEFT, 40, -20);
    lv_obj_t * label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, LV_SYMBOL_PREV);
    lv_obj_center(label_prev);

    lv_obj_t * btn_next = lv_btn_create(tile2);
    lv_obj_set_size(btn_next, 40, 40);
    lv_obj_align(btn_next, LV_ALIGN_BOTTOM_RIGHT, -40, -20);
    lv_obj_t * label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, LV_SYMBOL_NEXT);
    lv_obj_center(label_next);

    /* 设置初始Tile */
    lv_tileview_set_tile(tileview, tile1, LV_ANIM_OFF);

    /* 保存视图对象 */
    ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY].obj = gallery_view;
}

/* 销毁图库视图 */
static void destroy_gallery_view(void)
{
    lv_obj_t * gallery_view = ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY].obj;
    if(gallery_view) {
        lv_obj_del(gallery_view);
        ((struct {view_create_cb_t create_cb; view_destroy_cb_t destroy_cb; lv_obj_t * obj;} *)views)[VIEW_GALLERY].obj = NULL;
    }
}           

#endif // VIEW_MANAGER_H