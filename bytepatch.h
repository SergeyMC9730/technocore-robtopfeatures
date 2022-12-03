#pragma once

#include <stdint.h>
#include "techno.h"

#define AS_ADDRESS(x) reinterpret_cast<void *>(static_cast<uintptr_t>(x))

class PatchManager {
public:
    static PatchManager *getSharedState();

    void init();
    void patch(void *address, uint8_t byte);
private:
    PatchManager() { }
    static PatchManager *shared;
};