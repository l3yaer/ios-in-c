#include <CoreFoundation/CoreFoundation.h>
#include <objc/runtime.h>
#include <objc/message.h>

extern void UIApplicationMain(int, ...);

int main(int argc, char * argv[])
{
    id auto_release_pool = objc_msgSend(objc_msgSend(objc_getClass("NSAutoreleasePool"), sel_registerName("alloc")), sel_registerName("init"));

    UIApplicationMain(argc, argv, nil, CFSTR("AppDelegate"));

    objc_msgSend(auto_release_pool, sel_registerName("drain"));
}
