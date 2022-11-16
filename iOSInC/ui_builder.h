#ifndef ui_builder_h
#define ui_builder_h

#include <objc/runtime.h>
#include <CoreGraphics/CoreGraphics.h>

struct Drawable;

typedef struct CGRect (*msgRectFn)(id receiver, SEL operation, ...);
typedef struct CGPoint (*msgPointFn)(id receiver, SEL operation, ...);

typedef void (*OnTouchFn)(struct Drawable*);

typedef struct {
    CGFloat r, g, b, a;
} Color;

#define MAKE_COLOR(r, g, b, a) ((Color) {(r), (g), (b), (a)})
#define COLOR_WHITE MAKE_COLOR(1, 1, 1, 1)
#define COLOR_BLACK MAKE_COLOR(0, 0, 0, 1)
#define COLOR_RED MAKE_COLOR(1, 0, 0, 1)
#define COLOR_GREEN MAKE_COLOR(0, 1, 0, 1)
#define COLOR_BLUE MAKE_COLOR(0, 0, 1, 1)

#define MAKE_RECT(x, y, w, h) ((CGRect) {(x), (y), (w), (h)})
#define MAKE_POINT(x, y) ((CGPoint) {(x), (y)})
#define POINT_INVALID MAKE_POINT(CGFLOAT_MIN, CGFLOAT_MIN)

struct Drawable {
    CGRect frame;
    bool active;
    OnTouchFn touch_fn;

    enum {
        DRAWABLE_TYPE_NONE,
        DRAWABLE_TYPE_LABEL,
        DRAWABLE_TYPE_BUTTON
    } type;

    union {
        char *text;
    } data;
};

#define MAX_DRAWABLES 1024

struct UIData {
    CGPoint touch_loc;
    struct Drawable drawables[MAX_DRAWABLES];
    size_t drawables_count;
    Color background_color;
    Color text_color;
    Color primary_color;
    Color primary_light_color;
    Color primary_dark_color;
    CGRect canvas_rect;
    CGContextRef context;
};

void ui_init(void);

void draw_rect(CGRect rect, Color color);

void draw_text(const char *text, CGPoint origin);

void draw_button(const char *text, CGRect rect, OnTouchFn on_touch);

struct UIData *ui_data_ref(void);

void draw_start(void);

void draw_end(void);

#endif /* ui_builder_h */
