#include "./include/gd/include/gd.h"
#include "./include/dobby.h"
#include <dlfcn.h>
#include <chrono>
#include <ctime>
#include "./include/cocos2dx/include/cocos2d.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include "techno.h"

using namespace cocos2d;
using namespace cocos2d::extension;

namespace Techno {
    namespace TMenuLayer {
        bool (* init_o)(MenuLayer *);

        bool init(MenuLayer *self) {
            if(!init_o(self)) return false;

            auto tch = CCLabelBMFont::create("TechnoGDPS RELEASE 1.0", "bigFont.fnt");
            tch->setPositionY(301);
            tch->setPositionX(200);
            tch->setScale(.375f);
            tch->setAnchorPoint({});

            self->addChild(tch);

            return true;
        }

        void applyHooks() {
		    HOOK_FUNC("_ZN9MenuLayer4initEv");
	    }
    }
}

__attribute__((constructor)) void libinit(){
    Techno::TMenuLayer::applyHooks();
}