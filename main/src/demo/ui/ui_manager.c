#include "ui_manager.h"
#include "ui_views.h"
#include <stdio.h>

#define UI_STACK_MAX 16

#define UI_LOG(fmt, ...) \
    printf("[UI] " fmt "\r\n", ##__VA_ARGS__)

static ui_view_t g_views[VIEW_MAX] = {
    [VIEW_HOME] = {
        .id = VIEW_HOME,
        .init = view_home_init,
        .open = view_home_open,
        .pause = view_home_pause,
        .resume = view_home_resume,
        .close = view_home_close,
        .deinit = view_home_deinit,
    },
    [VIEW_SETTING] = {
        .id = VIEW_SETTING,
        .init = view_setting_init,
        .open = view_setting_open,
        .pause = view_setting_pause,
        .resume = view_setting_resume,
        .close = view_setting_close,
        .deinit = view_setting_deinit,
    },
    [VIEW_MUSIC] = {
        .id = VIEW_MUSIC,
        .init = view_music_init,
        .open = view_music_open,
        .pause = view_music_pause,
        .resume = view_music_resume,
        .close = view_music_close,
        .deinit = view_music_deinit,
    },
    [VIEW_WIFI] = {
        .id = VIEW_WIFI,
        .init = view_wifi_init,
        .open = view_wifi_open,
        .pause = view_wifi_pause,
        .resume = view_wifi_resume,
        .close = view_wifi_close,
        .deinit = view_wifi_deinit,
    },
    [VIEW_POPUP] = {
        .id = VIEW_POPUP,
        .init = view_popup_init,
        .open = view_popup_open,
        .pause = view_popup_pause,
        .resume = view_popup_resume,
        .close = view_popup_close,
        .deinit = view_popup_deinit,
    },
    [VIEW_ANDROID_LAUNCHER] = {
        .id = VIEW_ANDROID_LAUNCHER,
        .init = ui_android_init,
        .open = ui_android_open,
        .pause = ui_android_pause,
        .resume = ui_android_resume,
        .close = ui_android_close,
        .deinit = ui_android_deinit,
    },
    [VIEW_WATERFALL] = {
        .id = VIEW_WATERFALL,
        .init = ui_watchwaterfall_init,
        .open = ui_watchwaterfall_open,
        .pause = ui_watchwaterfall_pause,
        .resume = ui_watchwaterfall_resume,
        .close = ui_watchwaterfall_close,
        .deinit = ui_watchwaterfall_deinit,
    },
};

static view_id_t g_view_stack[UI_STACK_MAX];
static int g_stack_top = -1;
static bool g_manager_initialized = false;
static bool g_transitioning = false;

static bool view_id_valid(view_id_t id)
{
    return id >= VIEW_HOME && id < VIEW_MAX;
}

static view_id_t stack_top(void)
{
    if (g_stack_top < 0) {
        return VIEW_NONE;
    }

    return g_view_stack[g_stack_top];
}

static bool stack_push(view_id_t id)
{
    if (g_stack_top >= UI_STACK_MAX - 1) {
        UI_LOG("stack overflow, id=%d", id);
        return false;
    }

    g_view_stack[++g_stack_top] = id;
    return true;
}

static view_id_t stack_pop(void)
{
    if (g_stack_top < 0) {
        return VIEW_NONE;
    }

    return g_view_stack[g_stack_top--];
}

static void destroy_view(view_id_t id)
{
    if (!view_id_valid(id)) {
        return;
    }

    ui_view_t *view = &g_views[id];

    if (view->close != NULL) {
        view->close();
    }

    if (view->deinit != NULL) {
        view->deinit();
    }

    view->created = false;
}

void ui_push_view(view_id_t id)
{
    if (!view_id_valid(id)) {
        UI_LOG("invalid push id=%d", id);
        return;
    }

    if (g_transitioning) {
        UI_LOG("push rejected during transition, id=%d", id);
        return;
    }

    if (stack_top() == id) {
        UI_LOG("view %d already active", id);
        return;
    }

    if (g_stack_top >= UI_STACK_MAX - 1) {
        UI_LOG("push rejected: stack full");
        return;
    }

    g_transitioning = true;

    view_id_t old_id = stack_top();
    ui_view_t *old_view = view_id_valid(old_id) ? &g_views[old_id] : NULL;
    ui_view_t *new_view = &g_views[id];

    if (old_view != NULL && old_view->pause != NULL) {
        UI_LOG("View %d: pause", old_id);
        old_view->pause();
    }

    if (!new_view->created) {
        if (new_view->init != NULL) {
            UI_LOG("View %d: init", id);
            new_view->init();
        }

        new_view->created = true;
    }

    /*
     * Push the view before calling open().
     * This ensures lifecycle callbacks observe the correct active view.
     */
    if (!stack_push(id)) {
        g_transitioning = false;
        return;
    }

    if (new_view->open != NULL) {
        UI_LOG("View %d: open", id);
        new_view->open();
    }

    UI_LOG("active view=%d, depth=%d", id, g_stack_top + 1);
    g_transitioning = false;
}

void ui_pop_view(void)
{
    /*
     * The root view cannot be popped.
     * This guarantees that the stack always retains at least one root view.
     */
    if (g_stack_top <= 0) {
        UI_LOG("root view cannot be popped");
        return;
    }

    if (g_transitioning) {
        UI_LOG("pop rejected during transition");
        return;
    }

    g_transitioning = true;

    view_id_t old_id = stack_pop();
    view_id_t back_id = stack_top();

    UI_LOG("--- POP: remove %d, back to %d ---", old_id, back_id);

    destroy_view(old_id);

    if (view_id_valid(back_id)) {
        ui_view_t *back_view = &g_views[back_id];

        if (back_view->resume != NULL) {
            UI_LOG("View %d: resume", back_id);
            back_view->resume();
        }
    }

    g_transitioning = false;
}

void ui_switch_view(view_id_t id)
{
    if (!view_id_valid(id)) {
        UI_LOG("invalid switch id=%d", id);
        return;
    }

    if (g_transitioning) {
        UI_LOG("switch rejected during transition");
        return;
    }

    g_transitioning = true;

    /*
     * Clear all views directly instead of calling ui_pop_view(),
     * because ui_pop_view() must protect the root view.
     */
    while (g_stack_top >= 0) {
        view_id_t old_id = stack_pop();
        destroy_view(old_id);
    }

    g_transitioning = false;
    ui_push_view(id);
}

void ui_manager_init(void)
{
    if (g_manager_initialized) {
        UI_LOG("UI Manager already initialized");
        return;
    }

    UI_LOG("UI Manager init");

    g_stack_top = -1;
    ui_push_view(VIEW_HOME);

    g_manager_initialized = (g_stack_top >= 0);

    if (!g_manager_initialized) {
        UI_LOG("UI Manager initialization failed");
    }
}

void ui_manager_deinit(void)
{
    if (!g_manager_initialized) {
        return;
    }

    if (g_transitioning) {
        UI_LOG("deinit rejected during transition");
        return;
    }

    g_transitioning = true;

    while (g_stack_top >= 0) {
        view_id_t old_id = stack_pop();
        destroy_view(old_id);
    }

    g_manager_initialized = false;
    g_transitioning = false;

    UI_LOG("UI Manager deinitialized");
}

view_id_t ui_current_view(void)
{
    return stack_top();
}