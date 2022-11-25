#pragma once

#include "./include/dobby.h"

#define HOOK_FUNCX(fname, ffunc, ffunc_o) DobbyHook(dlsym(dlopen(GDSymbols::libraryName, RTLD_LAZY), fname), (void *)ffunc, (void **)&ffunc_o);
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