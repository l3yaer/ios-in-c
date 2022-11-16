#include <objc/runtime.h>
#include <objc/message.h>
#include <CoreGraphics/CoreGraphics.h>

#include "ui_builder.h"

Class ViewClass;

extern CGContextRef UIGraphicsGetCurrentContext();

unsigned int count = 0;

void inc_count(struct Drawable * d)
{
    count++;
}

static void view_draw_rect(id self, SEL _cmd, CGRect rect)
{
    ui_data_ref()->context = UIGraphicsGetCurrentContext();
    ui_data_ref()->canvas_rect = rect;

    char buf[8];

    snprintf(buf, 7, "%d", count);

    draw_start();
    draw_text(buf, MAKE_POINT(rect.size.width / 2.0 - 8, 150));
    draw_button("Inc", MAKE_RECT(rect.size.width / 2.0 - 100, 200, 200, 50), inc_count);
    draw_end();
}

static BOOL view_begin_track_touch(id self, SEL _cmd, id touch, id event)
{
    struct CGPoint location = ((msgPointFn) objc_msgSend)(touch, sel_getUid("locationInView:"), self);
    ui_data_ref()->touch_loc = location;
    objc_msgSend(self, sel_getUid("setNeedsDisplay"));

    return YES;
}

static void view_end_track_touch(id self, SEL _cmd, id touch, id event)
{
    ui_data_ref()->touch_loc = POINT_INVALID;
    objc_msgSend(self, sel_getUid("setNeedsDisplay"));
}

__attribute__((constructor))
static void initView()
{
    ViewClass = objc_allocateClassPair((Class) objc_getClass("UIControl"), "View", 0);
    class_addMethod(ViewClass, sel_getUid("drawRect:"), (IMP) view_draw_rect, "v@:");
    class_addMethod(ViewClass, sel_getUid("beginTrackingWithTouch:withEvent:"), (IMP) view_begin_track_touch, "@:@");
    class_addMethod(ViewClass, sel_getUid("endTrackingWithTouch:withEvent:"), (IMP) view_end_track_touch, "@:@");
    objc_registerClassPair(ViewClass);
    ui_init();
}
