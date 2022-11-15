#include <objc/runtime.h>
#include <CoreGraphics/CoreGraphics.h>

Class ViewClass;

extern CGContextRef UIGraphicsGetCurrentContext();

void View_drawRect(id self, SEL _cmd, CGRect rect)
{
    CGContextRef context = UIGraphicsGetCurrentContext();

    CGContextSetFillColor(context, (CGFloat []){1, 1, 1, 1});
    CGContextAddRect(context, rect);
    CGContextFillPath(context);
}

__attribute__((constructor))
static void initView()
{
    ViewClass = objc_allocateClassPair((Class) objc_getClass("UIView"), "View", 0);
    class_addMethod(ViewClass, sel_getUid("drawRect:"), (IMP) View_drawRect, "v@:");
    objc_registerClassPair(ViewClass);
}