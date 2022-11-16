#include "ui_builder.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFString.h>

struct UIData ui_data = (struct UIData) {
    POINT_INVALID,
    {0},
    0,
    MAKE_COLOR(0.086, 0.086, 0.086, 1),
    MAKE_COLOR(1, 1, 1, 1),
    MAKE_COLOR(0.160, 0.290, 0.478, 1),
    MAKE_COLOR(0.290, 0.360, 0.478, 1),
    MAKE_COLOR(0.129, 0.196, 0.305, 1),
};

CGFloat chars_width[256];
CGFloat chars_height;

#define FONT_NAME "Arial"
#define FONT_SIZE 12

void draw_rect(CGRect rect, Color color)
{
    CGContextSetFillColor(ui_data.context, (CGFloat []){color.r, color.g, color.b, color.a});
    CGContextFillRect(ui_data.context, rect);
}

struct UIData *ui_data_ref(void)
{
    return &ui_data;
}

void ui_init(void)
{
    CGFontRef font = CGFontCreateWithFontName(CFSTR(FONT_NAME));
    int units = CGFontGetUnitsPerEm(font);
    for(size_t i = 0; i < 256; ++i)
    {
        char character[2] = { (char) i, '\0' };
        CFStringRef name = CFStringCreateWithCString(NULL, character, kCFStringEncodingASCII);
        CGGlyph g = CGFontGetGlyphWithGlyphName(font, name);
        CGRect rect;
        CGFontGetGlyphBBoxes(font, &g, 1, &rect);
        chars_width[i] = (rect.size.width + rect.origin.x) / (CGFloat) units * FONT_SIZE;
    }
    chars_height = CGFontGetCapHeight(font)/ (CGFloat) units * (CGFloat) FONT_SIZE;
}

void render_debug_frame(CGRect frame)
{
    CGContextSetStrokeColor(ui_data.context, &COLOR_RED);
    CGContextStrokeRect(ui_data.context, frame);
}

size_t label_width(const char *text, size_t len)
{
    float width = 0.0;
    for(size_t i = 0; i < len; ++i)
        width += ceil(chars_width[text[i]]);

    return (size_t) ceilf(width);
}

void render_text(const char *text, CGRect frame)
{
    CGContextSetFillColor(ui_data.context,(CGFloat *) &ui_data.text_color);
    CGContextShowTextAtPoint(ui_data.context,
                             frame.origin.x, frame.origin.y + frame.size.height,
                             text,
                             strlen(text));
//    render_debug_frame(frame);
}

void render_label(struct Drawable drawable)
{
    render_text(drawable.data.text, drawable.frame);
}

CGSize text_size(const char *text)
{
    size_t width = label_width(text, strlen(text));
    return (CGSize) { (CGFloat) width, chars_height};
}

void render_button(struct Drawable drawable)
{
    if(drawable.active) {
        CGContextSetFillColor(ui_data.context,(CGFloat *) &ui_data.primary_light_color);
        drawable.touch_fn(&drawable);
    }
    else
        CGContextSetFillColor(ui_data.context,(CGFloat *) &ui_data.primary_color);

    CGContextSetStrokeColor(ui_data.context,(CGFloat *) &ui_data.primary_light_color);
    CGContextFillRect(ui_data.context, drawable.frame);
    CGContextStrokeRect(ui_data.context, drawable.frame);
    CGSize contet_size = text_size(drawable.data.text);
    render_text(drawable.data.text, MAKE_RECT(drawable.frame.size.width / 2.0 - contet_size.width / 2.0 + drawable.frame.origin.x,
                                              drawable.frame.size.height / 2.0 - contet_size.height / 2.0 + drawable.frame.origin.y,
                                              contet_size.width,
                                              contet_size.height));

}

void draw_start(void)
{
    ui_data.drawables_count = 0;
    draw_rect(ui_data.canvas_rect, ui_data.background_color);
    CGContextSetTextDrawingMode(ui_data.context, kCGTextFill);
    CGContextSelectFont(ui_data.context, FONT_NAME, FONT_SIZE, kCGEncodingMacRoman);
    CGAffineTransform affine = CGAffineTransformMake(1.0, 0.0, 0.0, -1.0, 0.0, 0.0);
    CGContextSetTextMatrix(ui_data.context, affine);
}

void draw_end(void)
{
    for (size_t i = 0; i < ui_data.drawables_count; ++i) {
        struct Drawable drawable = ui_data.drawables[i];
        switch (drawable.type) {
            case DRAWABLE_TYPE_LABEL:
                render_label(drawable);
                break;
            case DRAWABLE_TYPE_BUTTON:
                render_button(drawable);
                break;
            case DRAWABLE_TYPE_NONE:
            default:
                break;
        }
    }
}

bool point_is_inside_rect(CGPoint point, CGRect rect)
{
    return point.x >= rect.origin.x &&
    point.y >= rect.origin.y &&
    point.x <= rect.origin.x + rect.size.width &&
    point.y <= rect.origin.y + rect.size.height;
}

void draw_text(const char *text, CGPoint origin)
{
    struct Drawable drawable;
    drawable.data.text = strdup(text);
    drawable.type = DRAWABLE_TYPE_LABEL;
    CGSize size = text_size(drawable.data.text);
    drawable.frame = MAKE_RECT(origin.x, origin.y, size.width, size.height);
    drawable.active = point_is_inside_rect(ui_data.touch_loc, drawable.frame);
    ui_data.drawables[ui_data.drawables_count++] = drawable;
}

void draw_button(const char *text, CGRect rect, OnTouchFn on_touch)
{
    struct Drawable drawable;
    drawable.data.text = strdup(text);
    drawable.type = DRAWABLE_TYPE_BUTTON;
    drawable.frame = rect;
    drawable.touch_fn = on_touch;
    drawable.active = point_is_inside_rect(ui_data.touch_loc, drawable.frame);
    ui_data.drawables[ui_data.drawables_count++] = drawable;
}
