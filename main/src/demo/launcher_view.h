
#include "../../../lvgl/lvgl.h"

// Define the number of app pages
#define APP_PAGE_COUNT 3
// Define the number of apps per page
#define APPS_PER_PAGE 9

// App information structure for the 3x3 grid
typedef struct {
    const char * icon;
    const char * label;
} app_info_t_9;

// 3x3 grid app list (renamed to app_list_9)
app_info_t_9 app_list_9[9] = {
    {LV_SYMBOL_WIFI, "WiFi"},
    {LV_SYMBOL_BLUETOOTH, "Bluetooth"},
    {LV_SYMBOL_BATTERY_FULL, "Battery"},
    {LV_SYMBOL_AUDIO, "Music"},
    {LV_SYMBOL_IMAGE, "Gallery"},
    {LV_SYMBOL_FILE, "Files"},
    {LV_SYMBOL_BELL, "Alarms"},
    {LV_SYMBOL_EDIT, "Notes"},
    {LV_SYMBOL_SETTINGS, "Settings"}
};

// App click event handler
static void app_click_event_handler_9(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    int index = (int)lv_obj_get_user_data(btn);
    printf("Clicked app: %s\n", app_list_9[index].label); // Updated reference
}

// Create 3x3 grid interface
void create_nine_grid_interface(lv_obj_t * parent) {
    // Create a container for the 3x3 grid
    lv_obj_t * container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));
    lv_obj_center(container);

    // Define grid layout
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(container, col_dsc, row_dsc);

    // Iterate through the app list
    for (int i = 0; i < 9; i++) {
        int row = i / 3;
        int col = i % 3;

        // Create button
        lv_obj_t * btn = lv_btn_create(container);
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1,
                             LV_GRID_ALIGN_CENTER, row, 1);
        lv_obj_set_size(btn, 80, 80);
        lv_obj_set_user_data(btn, (void *)i);

        // Set button style
        static lv_style_t style_btn;
        lv_style_init(&style_btn);
        lv_style_set_bg_color(&style_btn, lv_color_hex(0x303030));
        lv_style_set_bg_opa(&style_btn, LV_OPA_80);
        lv_style_set_radius(&style_btn, 20);
        lv_style_set_border_width(&style_btn, 2);
        lv_style_set_border_color(&style_btn, lv_color_hex(0x505050));
        lv_obj_add_style(btn, &style_btn, 0);

        // Button pressed style
        static lv_style_t style_btn_pr;
        lv_style_init(&style_btn_pr);
        lv_style_set_bg_color(&style_btn_pr, lv_color_hex(0x404040));
        lv_obj_add_style(btn, &style_btn_pr, LV_STATE_PRESSED);

        // Create label
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "  %s\n%s", app_list_9[i].icon, app_list_9[i].label); // Updated reference
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

        // Add click event
        lv_obj_add_event_cb(btn, app_click_event_handler_9, LV_EVENT_CLICKED, NULL);
    }
}


// Define view ID
typedef enum {
    VIEW_APP1_E,
    VIEW_APP2_E,
    VIEW_APP3_E,
    VIEW_APPCOUNT_E
} view_id_app;

static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
// Current active view
static view_id_app launcher_top_view = VIEW_APP1_E;

// Function prototypes
static void create_top_status_bar(lv_obj_t * parent);
static void create_middle_app_icons(lv_obj_t * parent);
static void create_bottom_icons(lv_obj_t * parent);
static void launcher_switch_to(view_id_app view_id);
static void app_click_event_handler(lv_event_t * e);

// View registration table
static struct {
    lv_obj_t * obj;
} launcher_views[VIEW_APPCOUNT_E] = {
    [VIEW_APP1_E] = {NULL},
    [VIEW_APP2_E] = {NULL},
    [VIEW_APP3_E] = {NULL}
};

// App information structure
typedef struct {
    const char * icon;
    const char * label;
    view_id_app view_id;
} app_info_t;

// App list
app_info_t app_list[APPS_PER_PAGE * APP_PAGE_COUNT] = {
    // Page 1
    {LV_SYMBOL_SETTINGS, "Settings", VIEW_APP1_E},
    {LV_SYMBOL_AUDIO, "Music", VIEW_APP1_E},
    {LV_SYMBOL_IMAGE, "Gallery", VIEW_APP1_E},
    {LV_SYMBOL_FILE, "Files", VIEW_APP1_E},
    {LV_SYMBOL_BELL, "Alarms", VIEW_APP1_E},
    {LV_SYMBOL_EDIT, "Notes", VIEW_APP1_E},
    {LV_SYMBOL_BATTERY_FULL, "Battery", VIEW_APP1_E},
    {LV_SYMBOL_WIFI, "WiFi", VIEW_APP1_E},
    {LV_SYMBOL_BLUETOOTH, "Bluetooth", VIEW_APP1_E},
    // Page 2
    {LV_SYMBOL_HOME, "Home", VIEW_APP2_E},
    {LV_SYMBOL_CLOSE, "Close", VIEW_APP2_E},
    {LV_SYMBOL_OK, "OK", VIEW_APP2_E},
    {LV_SYMBOL_LEFT, "Left", VIEW_APP2_E},
    {LV_SYMBOL_RIGHT, "Right", VIEW_APP2_E},
    {LV_SYMBOL_UP, "Up", VIEW_APP2_E},
    {LV_SYMBOL_DOWN, "Down", VIEW_APP2_E},
    {LV_SYMBOL_PLAY, "Play", VIEW_APP2_E},
    {LV_SYMBOL_PAUSE, "Pause", VIEW_APP2_E},
    // Page 3
    {LV_SYMBOL_STOP, "Stop", VIEW_APP3_E},
    {LV_SYMBOL_REFRESH, "Refresh", VIEW_APP3_E},
    {LV_SYMBOL_TRASH, "Trash", VIEW_APP3_E},
    {LV_SYMBOL_SAVE, "Save", VIEW_APP3_E},
    {LV_SYMBOL_OK, "Open", VIEW_APP3_E},
    {LV_SYMBOL_KEYBOARD, "Keyboard", VIEW_APP3_E},
    {LV_SYMBOL_GPS, "Camera", VIEW_APP3_E},
    {LV_SYMBOL_AUDIO, "Phone", VIEW_APP3_E},
    {LV_SYMBOL_SD_CARD, "Message", VIEW_APP3_E}
};

// Main function to initialize the launcher
void launcher_init(void) {
    lv_obj_t * scr = lv_scr_act();

    // Create top status bar
    create_top_status_bar(scr);

    // Create middle app icons
    create_middle_app_icons(scr);

    // Create bottom icons
    create_bottom_icons(scr);

    // Switch to the first view
    launcher_switch_to(VIEW_APP1_E);
}

// Create the top status bar
static void create_top_status_bar(lv_obj_t * parent) {
    lv_obj_t * status_bar = lv_obj_create(parent);
    lv_obj_set_size(status_bar, LV_PCT(100), 30);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);

    // Create time label
    lv_obj_t * time_label = lv_label_create(status_bar);
    lv_label_set_text(time_label, "12:00");
    lv_obj_align(time_label, LV_ALIGN_LEFT_MID, 10, 0);

    // Create wifi and battery label
    lv_obj_t * wifi_battery_label = lv_label_create(status_bar);
    lv_label_set_text(wifi_battery_label, LV_SYMBOL_WIFI " " LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(wifi_battery_label, LV_ALIGN_RIGHT_MID, -10, 0);
}

// Create the middle app icons
static void create_middle_app_icons(lv_obj_t * parent) {
    lv_obj_t * tv= lv_tileview_create(parent);
    lv_obj_set_size(tv, LV_PCT(100), LV_PCT(100) - 20);
    lv_obj_align(tv, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_grid_dsc_array(tileview, col_dsc, row_dsc);


    /*Tile1: just a label*/
    lv_obj_t * tile1 = lv_tileview_add_tile(tv, 0, 0, LV_DIR_BOTTOM);
    create_nine_grid_interface(tile1);
    lv_obj_t * label = lv_label_create(tile1);
    /*
    lv_label_set_text(label, "Scroll down");
    lv_obj_center(label);
    */
    /*Tile2: a button*/
    lv_obj_t * tile2 = lv_tileview_add_tile(tv, 0, 1, (lv_dir_t)(LV_DIR_TOP | LV_DIR_RIGHT));
    lv_obj_t * btn   = lv_button_create(tile2);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Scroll up or right");
    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(btn);

    /*Tile3: a list*/
    lv_obj_t * tile3 = lv_tileview_add_tile(tv, 1, 1, (lv_dir_t)(LV_DIR_LEFT| LV_DIR_RIGHT));
    create_nine_grid_interface(tile3);
    /*
    lv_obj_t * label3 = lv_label_create(tile3);

    lv_label_set_text(label3, "label");
    lv_obj_center(label3);
    */

    /*Tile4: a list*/
    lv_obj_t * tile4 = lv_tileview_add_tile(tv, 2, 1, (lv_dir_t)(LV_DIR_LEFT|LV_DIR_RIGHT));
    lv_obj_t * list  = lv_list_create(tile4);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));

    lv_list_add_button(list, NULL, "One");
    lv_list_add_button(list, NULL, "Two");
    lv_list_add_button(list, NULL, "Three");
    lv_list_add_button(list, NULL, "Four");
    lv_list_add_button(list, NULL, "Five");
    lv_list_add_button(list, NULL, "Six");
    lv_list_add_button(list, NULL, "Seven");
    lv_list_add_button(list, NULL, "Eight");
    lv_list_add_button(list, NULL, "Nine");
    lv_list_add_button(list, NULL, "Ten");
        #if 0
    // Create tiles for each page
    for (int page = 0; page < APP_PAGE_COUNT; page++) {
        lv_obj_t * tile = lv_tileview_add_tile(tileview, page, 0, (lv_dir_t)(LV_DIR_LEFT | LV_DIR_RIGHT));

        lv_obj_t * label = lv_label_create(tile);
        char page_str[10];
        snprintf(page_str, sizeof(page_str), "page %d", page);
        lv_label_set_text(label, page_str);
        lv_obj_center(label);

        switch(page){
            case 0:
                lv_obj_set_style_bg_color(tile, lv_color_hex(0xFFE4E1), 0);
                printf("page 0\n");
                break;
            case 1:
                lv_obj_set_style_bg_color(tile, lv_color_hex(0xFFFFFF), 0);
                printf("page 1\n");
                break;
            case 2:
                lv_obj_set_style_bg_color(tile, lv_color_hex(0xFFFFF1), 0);
                printf("page 2\n");
                break;
            case 3:
                lv_obj_set_style_bg_color(tile, lv_color_hex(0xFFFAA1), 0);
                printf("page 3\n");
                break;
            default:
                lv_obj_set_style_bg_color(tile, lv_color_hex(0xFF1111), 0);
                printf("page 4\n");
                break;  

        }
        // Create app icons for each page
        for (int i = 0; i < APPS_PER_PAGE; i++) {
            int index = page * APPS_PER_PAGE + i;
            int row = i / 3;
            int col = i % 3;

            // Create the button
            lv_obj_t * btn = lv_btn_create(tile);
            lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1,
                                 LV_GRID_ALIGN_CENTER, row, 1);
            lv_obj_set_size(btn, 80, 80);

            // Set the button style
            static lv_style_t style_btn;
            lv_style_init(&style_btn);
            lv_style_set_bg_color(&style_btn, lv_color_hex(0x303030));
            lv_style_set_bg_opa(&style_btn, LV_OPA_80);
            lv_style_set_radius(&style_btn, 20);
            lv_style_set_border_width(&style_btn, 2);
            lv_style_set_border_color(&style_btn, lv_color_hex(0x505050));
            lv_obj_add_style(btn, &style_btn, 0);

            // Button pressed style
            static lv_style_t style_btn_pr;
            lv_style_init(&style_btn_pr);
            lv_style_set_bg_color(&style_btn_pr, lv_color_hex(0x404040));
            lv_obj_add_style(btn, &style_btn_pr, LV_STATE_PRESSED);

            // Create the label
            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text_fmt(label, "  %s\n%s", app_list[index].icon, app_list[index].label);
            lv_obj_center(label);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

            // Add the click event
            lv_obj_add_event_cb(btn, app_click_event_handler, LV_EVENT_CLICKED, (void*)app_list[index].view_id);
        }

        launcher_views[page].obj = tile;
    }
            #endif
}

// Create the bottom icons
static void create_bottom_icons(lv_obj_t * parent) {
    lv_obj_t * bottom_bar = lv_obj_create(parent);
    lv_obj_set_size(bottom_bar, LV_PCT(100), 50);
    lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(bottom_bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(bottom_bar, LV_OBJ_FLAG_SCROLLABLE);

    const char * icons[] = {LV_SYMBOL_OK, LV_SYMBOL_GPS, LV_SYMBOL_WIFI, LV_SYMBOL_SD_CARD};

    for (int i = 0; i < 4; i++) {
        lv_obj_t * btn = lv_btn_create(bottom_bar);
        lv_obj_set_size(btn, 60, 30);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, i * 80 + 10, 0);

        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "  %s  ", icons[i]);
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);

        // Add the click event
        lv_obj_add_event_cb(btn, app_click_event_handler, LV_EVENT_CLICKED, (void*)VIEW_APP1_E);
    }
}

// Switch to the specified view
static void launcher_switch_to(view_id_app view_id) {
    // Hide all views
    for (int i = 0; i < VIEW_APPCOUNT_E; i++) {
        if (launcher_views[i].obj) {
            lv_obj_add_flag(launcher_views[i].obj, LV_OBJ_FLAG_HIDDEN);
        }
    }

    // Update the current view ID
    launcher_top_view = view_id;

    // Show the target view
    if (launcher_views[view_id].obj) {
        lv_obj_clear_flag(launcher_views[view_id].obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(launcher_views[view_id].obj);
    } else {
        printf("Error: View object is NULL for view_id: %d\n", view_id);
    }
}

// Icon click event handler
static void app_click_event_handler(lv_event_t * e) {
    view_id_app view_id = (view_id_app)lv_event_get_user_data(e);
    launcher_switch_to(view_id);
}