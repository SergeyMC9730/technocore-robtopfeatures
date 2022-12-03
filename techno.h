#pragma once

#ifdef __ANDROID__
#include <dlfcn.h>
#include "./include/dobby.h"
#include "./include/gd/include/gd.h"
#include "./include/cocos2dx/include/cocos2d.h"
// #include <cpu-features.h>
#include "bytepatch.h"

#define HOOK_FUNCX(fname, ffunc, ffunc_o) DobbyHook(dlsym(dlopen("libcocos2dcpp.so", RTLD_LAZY), fname), (void *)ffunc, (void **)&ffunc_o);
#define HOOK_FUNC(fname) HOOK_FUNCX(fname, init, init_o);
#define HOOK_PROTECTOR if(!init_o(self)) return false

namespace GDSymbols {
    extern const char *libraryName;
    namespace MenuLayer {
        const char *init(void *menuLayerClass);
    }
    namespace FLAlertLayer {
        const char *create(FLAlertLayerProtocol *delegate, char *title, std::string contents, const char *btn0, const char *btn2);
    }
    
}
#endif

#ifdef _WIN32
#include "win32cac.h"
#include "MinHook.h"
#endif

#if __APPLE__
	#include <CacKit>
#endif

using namespace cocos2d;