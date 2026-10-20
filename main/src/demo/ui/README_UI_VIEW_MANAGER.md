# UI View Manager — Developer Guide

A small, layered view manager for LVGL 9.x. It gives you a navigation
stack (`push` / `pop` / `switch`) with a clean per-view lifecycle, and lets
you add a new screen in a few lines.

- **Source root:** `main/src/demo/ui/`
- **Entry point:** `ui_views_register_all()` + `ui_manager_init()` in `main/src/main.c`

---

## 1. Why this structure

The whole idea is **layering**: each file has exactly one job.

```
 +--------------------------------------------------------------+
 ¦ Layer 4  examples   ui_*_view.c / mobile/ui_*.c              ¦
 ¦          one file per view — widgets only                    ¦
 +--------------------------------------------------------------¦
 ¦ Layer 3  page base  ui_page_base.[ch]                        ¦
 ¦          shared container + default lifecycle                ¦
 +--------------------------------------------------------------¦
 ¦ Layer 2  domain     ui_views.[ch]                            ¦
 ¦          view ids, ui_view_t descriptor, registry            ¦
 +--------------------------------------------------------------¦
 ¦ Layer 1  framework  ui_manager.[ch]                          ¦
 ¦          view stack + lifecycle engine (view-agnostic)       ¦
 +--------------------------------------------------------------+
```

| File | Layer | Responsibility |
|------|-------|----------------|
| `ui_manager.c` | framework | Owns the stack, the re-entrancy guard and per-view `created` state. Knows **nothing** about concrete views. |
| `ui_manager.h` | framework | Public navigation API. |
| `ui_views.c` | domain | The **only** place that maps `VIEW_*` ids to view functions. |
| `ui_views.h` | domain | `view_id_t`, `ui_view_t`, `UI_VIEW_DECLARE()`. |
| `ui_theme.h` | design | Palette, fonts, layout tokens and shared style helpers. |
| `ui_page_base.c` | page base | Root object, header/back button, content area, default lifecycle. |
| `ui_page_base.h` | page base | `ui_page_t`, `UI_VIEW_DEFINE()`. |
| `ui_*_view.c` | examples | Widgets of one screen. |

The framework never changes when you add a screen; you only touch
`ui_views.[ch]` and add one `.c` file.

---

## 2. Quick start

### main.c

```c
ui_views_register_all();   /* publish every view to the manager */
ui_manager_init();         /* push VIEW_HOME as the root view   */
```

`ui_views_register_all()` must run **before** `ui_manager_init()`.

---

## 3. The view lifecycle

Every view implements up to six callbacks. All of them are optional.

| Callback | When it runs | Typical use |
|----------|--------------|-------------|
| `init`   | once, before the view is first shown | build widgets |
| `open`   | the view became the active (top) view | show, start animations |
| `pause`  | another view was pushed on top | hide, pause timers |
| `resume` | the view is revealed again by a `pop` | show, refresh data |
| `close`  | the view left the stack for good | hide |
| `deinit` | right after `close`, once | free widgets |

The manager drives this order:

```
push(id):   current.pause  ->  new.init (only once)  ->  stack push  ->  new.open
pop():      old.close      ->  old.deinit           ->  back.resume
switch(id): close+deinit every view on the stack    ->  push(id)
```

The view is pushed onto the stack **before** `open()` runs, so callbacks
can always call `ui_current_view()` and see the correct active view.

The root view (pushed by `ui_manager_init()`) can never be popped, so
`ui_current_view()` never returns `VIEW_NONE` while the UI is running.

---

## 4. Public API

From `ui_manager.h`:

```c
bool ui_manager_register(const ui_view_t *view); /* call before init()  */
bool ui_manager_init(void);                      /* pushes VIEW_HOME    */
void ui_manager_deinit(void);                    /* close + free all    */

void     ui_push_view(view_id_t id);             /* go to id, keep stack */
void     ui_pop_view(void);                      /* back, drop top view  */
void     ui_switch_view(view_id_t id);           /* reset stack to id    */
view_id_t ui_current_view(void);                 /* active view, or NONE */
```

Navigation rules:

- `ui_push_view()` is ignored if `id` is already the active view.
- Navigation started from inside a lifecycle callback (i.e. while a
  transition is running) is rejected and logged — this prevents re-entrant
  stack corruption. If you need to navigate from an event, call it from
  the LVGL event callback (outside a transition), never from `open`/`pause`.
- The stack holds up to `UI_STACK_MAX` (16) views.

---

## 5. Adding a new view

Say you want a `VIEW_SENSORS` screen.

**1. Add the id** in `ui_views.h`:

```c
typedef enum {
    VIEW_NONE = -1,
    VIEW_HOME = 0,
    ...
    VIEW_WATERFALL,
    VIEW_SENSORS,      /* <-- new, before VIEW_MAX */
    VIEW_MAX
} view_id_t;
```

**2. Declare the lifecycle** next to the others in `ui_views.h`:

```c
UI_VIEW_DECLARE(view_sensors);
```

**3. Create the view file** `main/src/demo/ui/ui_sensors_view.c`:

```c
#include "ui_views.h"
#include "ui_page_base.h"

static ui_page_t s_page;

static void sensors_build(ui_page_t *page)
{
    ui_page_create_root(page, "SENSORS", UI_COLOR_BG);
    lv_obj_t *content = ui_page_create_header(page, "Sensors", true);

    /* your widgets go inside `content` ... */
}

UI_VIEW_DEFINE(view_sensors, &s_page, sensors_build)
```

**4. Register it** in `ui_views.c`:

```c
UI_VIEW_DESC(VIEW_SENSORS, view_sensors);
```

and add `&view_sensors_desc` to `g_views[]`.

**5. Add the file to `CMakeLists.txt`** (both the FreeRTOS and the normal
`add_executable(main ...)` blocks).

That's it — the manager picks it up automatically.

---

## 6. `ui_page_base` reference

The page is a full-screen container owned by a view.

```c
lv_obj_t *ui_page_create_root(ui_page_t *page, const char *name, lv_color_t bg);

/* Title bar + optional back arrow; returns the content area below it. */
lv_obj_t *ui_page_create_header(ui_page_t *page, const char *title,
                                bool show_back);

void      ui_page_create_title(ui_page_t *page, const char *title);
void      ui_page_create_navigation(ui_page_t *page, const char *next_text,
                                    view_id_t next_id, bool show_back);
void      ui_page_show(ui_page_t *page);
void      ui_page_hide(ui_page_t *page);
void      ui_page_destroy(ui_page_t *page);
```

Default lifecycle implemented for you:

```c
void ui_page_view_init  (ui_page_t *page, ui_page_build_t build);
void ui_page_view_open  (ui_page_t *page, ui_page_build_t build);
void ui_page_view_pause (ui_page_t *page);
void ui_page_view_resume(ui_page_t *page);
void ui_page_view_close (ui_page_t *page);
void ui_page_view_deinit(ui_page_t *page);
```

`UI_VIEW_DEFINE(prefix, &page, build)` wires those six functions to the
names declared by `UI_VIEW_DECLARE(prefix)` so a normal page never repeats
the lifecycle boilerplate.

**Widgets-only build (no page):** if your view manages its own LVGL
objects (like the waterfall), skip `UI_VIEW_DEFINE` and write the six
functions yourself. See `mobile/ui_watch_waterfall.c`.

---

## 7. Theme (`ui_theme.h`)

All colours, fonts and shared styling live in one header, so every view
looks the same and a restyle is a single-file change.

```c
/* Palette */
UI_COLOR_BG  UI_COLOR_SURFACE  UI_COLOR_TILE  UI_COLOR_TILE_PRESS
UI_COLOR_ACCENT  UI_COLOR_ACCENT_2  UI_COLOR_TEXT  UI_COLOR_TEXT_DIM

/* Fonts */
UI_FONT_DISPLAY  UI_FONT_TITLE  UI_FONT_BODY  UI_FONT_SMALL
UI_FONT_ICON  UI_FONT_ICON_LARGE

/* Layout tokens */
UI_PAD  UI_GAP  UI_RADIUS_CARD  UI_RADIUS_TILE  UI_TOUCH_SIZE

/* Helpers */
ui_style_surface(obj, color);        /* flat non-scrollable surface      */
ui_style_transparent(obj);           /* invisible grouping container     */
ui_style_flex(obj, flow, main, cross, gap);
ui_label(parent, text, color, font);
ui_card(parent, color);              /* rounded surface + soft shadow    */
ui_divider(parent, width);
```

A page header is the standard way to get consistent chrome **and** a
working back button:

```c
lv_obj_t *content = ui_page_create_header(page, "Title", true /* back */);
/* add widgets to `content` */
```

The root view (HOME) passes `show_back = false`; every pushed view passes
`true`, so there is always a way back to home.

---

## 8. Included example views

| View id | File | Notes |
|---------|------|-------|
| `VIEW_HOME` | `ui_home_view.c` | Root view (no back button). Status bar, gradient clock card, flex-wrapped quick-launch grid driven by the `s_tiles[]` table. |
| `VIEW_SETTING` | `ui_setting_view.c` | Header + card. |
| `VIEW_MUSIC` | `ui_music_view.c` | Header + "now playing" card. |
| `VIEW_WIFI` | `ui_wifi_view.c` | Header + connection status card. |
| `VIEW_POPUP` | `ui_popup_view.c` | Header + modal style card with a Close button. |
| `VIEW_ANDROID_LAUNCHER` | `mobile/ui_android.c` | Self-managed widgets: swipeable app pages, page dots, nav bar **and a back button**. |
| `VIEW_WATERFALL` | `mobile/ui_watch_waterfall.c` | Interactive zoom grid: staggered ("honeycomb") tile layout, tiles zoom by distance from the centre and pan by dragging. Ported from `lv_demo_waterfall()` with all image assets removed. |

All non-root views have a **back button** in their header, so every screen
can return to home. Swipe / drag gestures (waterfall, launcher pages) and
tap targets coexist; if you add a drag-heavy view, note that `LV_EVENT_PRESSING`
is used for panning, while buttons use `LV_EVENT_CLICKED`.

### The waterfall (default demo)

`VIEW_HOME` is the root screen; tapping the **Watch** tile pushes
`VIEW_WATERFALL`, which is the real waterfall view (it replaced the old
placeholder `ui_watch_view.c`). Drag anywhere to pan the tile grid, click
a tile to open a message box, and use `ui_pop_view()` (Back) to return.

The zoom effect is ported from `lv_demo_waterfall()` (see
`main/src/demo/view_watefall.h`) but uses **no image resources**. The two
parts of the algorithm:

**1. Staggered grid.** Tiles sit on a brick/honeycomb pattern — every odd
row is shifted half a cell to the right, and rows are spaced by
`ICON_SIZE * 0.866` (sin 60°):

```c
int x = col * ICON_SIZE + ICON_SIZE / 2;
if (row % 2) x += ICON_SIZE / 2;           /* stagger odd rows */
int y = (int)(row * ICON_SIZE * ROW_STEP) + ICON_SIZE / 2;
```

Each tile's home position is stored in `s_home[]` once, so panning just
re-adds the drag offset instead of recomputing the grid.

**2. Distance zoom.** Each tile is scaled by how close it is to the screen
centre. Scaling uses the LVGL transform style, where **256 == 100%**, so
the widget is scaled at render time rather than being resized:

```c
t     = 1 - dist / ICON_FALLOFF            /* 1 at centre, 0 far away */
scale = SCALE_MIN + t * (SCALE_MAX - SCALE_MIN)   /* 15 .. 240 */

lv_obj_set_style_transform_scale(tile, scale, 0);
```

Because `transform_scale` scales around the widget's centre, the tile
stays centred on its layout position — no width arithmetic needed. The
nearest tiles are also raised with `lv_obj_move_foreground()` so they
overlap their neighbours correctly.

Instead of image assets, every tile is a plain rounded `lv_obj_t` with a
background colour and the same built-in LVGL icon glyph
(`LV_SYMBOL_IMAGE`) centred inside it, so the effect uses nothing but the
LVGL core:

```c
lv_obj_t *glyph = lv_label_create(tile);
lv_label_set_text(glyph, LV_SYMBOL_IMAGE);   /* same icon on every tile */
lv_obj_center(glyph);
```


---

## 8. Logging

Every transition is logged on `stdout` with a `[UI]` prefix, using view
names instead of numbers:

```
[UI] registered VIEW_HOME
[UI] UI Manager init
[UI] VIEW_HOME: init
[UI] VIEW_HOME: open
[UI] active=VIEW_HOME depth=1
[UI] VIEW_HOME: pause
[UI] VIEW_WATERFALL: init
[UI] VIEW_WATERFALL: open
[UI] active=VIEW_WATERFALL depth=2
```

The log is the fastest way to confirm that a lifecycle bug is in the
manager or in your view.

---

## 9. Naming convention

A view is identified by a **prefix** passed to `UI_VIEW_DECLARE` /
`UI_VIEW_DEFINE`, which expands to the six functions
`<prefix>_init/open/pause/resume/close/deinit`:

- normal pages: `view_home`, `view_setting`, ...
- self-managed views: `ui_android`, `ui_watch_waterfall`

Keep the prefix identical in `ui_views.h`, `ui_views.c` and the view file.
