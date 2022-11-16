#include <objc/runtime.h>
#include <objc/message.h>
#include <CoreGraphics/CoreGraphics.h>

#include "ui_builder.h"

struct AppDel {
    Class isa;

    id window;
};

static BOOL did_finish_launching(struct AppDel *self, SEL _cmd, void *application, void *options)
{
    struct CGRect screen_bounds = ((msgRectFn) objc_msgSend_stret)(objc_msgSend(objc_getClass("UIScreen"), sel_getUid("mainScreen")), sel_getUid("bounds"));

    self->window = objc_msgSend(objc_getClass("UIWindow"), sel_getUid("alloc"));
    self->window = objc_msgSend(self->window, sel_getUid("initWithFrame:"), screen_bounds);

    id viewController = objc_msgSend(objc_msgSend(objc_getClass("UIViewController"), sel_getUid("alloc")), sel_getUid("init"));

    id view = objc_msgSend(objc_msgSend(objc_getClass("View"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), screen_bounds);

    objc_msgSend(objc_msgSend(viewController, sel_getUid("view")), sel_getUid("addSubview:"), view);
    objc_msgSend(self->window, sel_getUid("setRootViewController:"), viewController);

    objc_msgSend(self->window, sel_getUid("makeKeyAndVisible"));
    return YES;
}

Class AppDelClass;

__attribute__((constructor))
static void initAppDel()
{
    AppDelClass = objc_allocateClassPair((Class) objc_getClass("UIResponder"), "AppDelegate", 0);

    class_addIvar(AppDelClass, "window", sizeof(id), 0, "@");

    class_addMethod(AppDelClass, sel_getUid("application:didFinishLaunchingWithOptions:"), (IMP) did_finish_launching, "i@:@@");

    objc_registerClassPair(AppDelClass);
}
