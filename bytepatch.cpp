#include "bytepatch.h"

#include <algorithm>
#include <fstream>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

#include <android/log.h>

#include "include/cocos2dx/include/cocos2d.h"
#include "include/gd/include/gd.h"

#define memoryPage(x) reinterpret_cast<uintptr_t>((x) & ~(GLOBAL_PAGESIZE - 1))

void* GLOBAL_HANDLE = nullptr;
void* GLOBAL_BASE = nullptr;
size_t GLOBAL_PAGESIZE;

PatchManager *PatchManager::shared = nullptr;

PatchManager *PatchManager::getSharedState() {
    if(!shared) shared = new PatchManager;
    return shared;
}

void PatchManager::init() {
    Dl_info p;
    dladdr(reinterpret_cast<void *>(&AppDelegate::get), &p);
    __android_log_print(ANDROID_LOG_DEBUG, "TechnoGDPS.21", "FBASE: 0x%00000000X", (int)p.dli_fbase);
    GLOBAL_BASE = p.dli_fbase;
    auto handle = dlopen("libcocos2dcpp.so", RTLD_NOW);
    GLOBAL_HANDLE = handle;
    GLOBAL_PAGESIZE = sysconf(_SC_PAGESIZE);
    __android_log_print(ANDROID_LOG_DEBUG, "TechnoGDPS.21", "Handle: 0x%00000000X", (int)GLOBAL_HANDLE);
    __android_log_print(ANDROID_LOG_DEBUG, "TechnoGDPS.21", "Page size: %d", (int)GLOBAL_HANDLE, GLOBAL_PAGESIZE);
}

void PatchManager::patch(void *address, uint8_t byte) {
    auto ptr_page = memoryPage(reinterpret_cast<uintptr_t>(address));
    int ret = mprotect(reinterpret_cast<void *>(ptr_page), GLOBAL_PAGESIZE, PROT_WRITE);
    if(ret == 0) {
        std::memset(address, byte, 1);
        mprotect(reinterpret_cast<void *>(ptr_page), GLOBAL_PAGESIZE, PROT_READ | PROT_EXEC);
    } else {
        __android_log_print(ANDROID_LOG_DEBUG, "TechnoGDPS.21", "Failed to patch address 0x%00000000X to byte 0x%00X (0x%00000000X) with %d", (uint32_t)address, (uint32_t)byte, ptr_page, ret);
    }

    return;
}