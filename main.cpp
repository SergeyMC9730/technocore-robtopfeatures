#define CAC_PROJ_NAME "Template"

#include "techno.h"
#include "LevelTools.h"
#include "CreatorLayer.h"
// #include "GJDialogObject.h"

#ifndef __ANDROID__
namespace Cvolton {
	namespace MHook {
		using std::uintptr_t;

		inline std::unordered_map<void*, void*> hooks;

		auto registerHook(uintptr_t address, void* hook) {
			void* trampoline;
			auto status = MH_CreateHook(reinterpret_cast<void**>(address), hook, &trampoline);
			if (status == MH_OK)
				hooks[hook] = trampoline;
			else if(MessageBoxA(nullptr, "An error has occured while hooking a function...\n\nThis is likely caused by an outdated version of \"minhook.x86.dll\".\nWould you like to open the download page and get the updated version?\n\nFix tutorial:\n1) Download minhook.x86.dll from this page\n2) Put it in your GD folder", "BetterInfo - Geometry Dash", MB_ICONERROR | MB_YESNO) == IDYES) system("explorer https://github.com/HJfod/minhook/releases/latest");
			return status;
		}

		template <typename F>
		inline auto getOriginal(F self) {
			return reinterpret_cast<F>(hooks[self]);
		}
	}
}
#endif

namespace Techno {
	namespace TMessage {
		void test(CCLayer *l) {
			#ifndef __ANDROID__
			// DialogObject *dobj = GJDialogObject::create("123", "456", 5, 1.f, false, {100, 100, 100});
			// l->addChild((CCNode *)dobj);
			#endif
		}
	}
	namespace TMenuLayer {
		#ifndef __ANDROID__
		class $implement(MenuLayer, MyMenuLayer) {
		public:
			static inline bool(__thiscall * _init)(MenuLayer * self);

			void buttonCallback(CCObject * sender) {
				auto alert = FLAlertLayer::create(NULL, "Mod", "Ok", NULL, "<cg>custom button!</c>");
				alert->show();
			}

			bool inithook() {
				printf("inithook\n");

				if (!_init(this))
					return false;

				TMessage::test((CCLayer *)this);

				auto tch = CCLabelBMFont::create("TechnoGDPS RELEASE 1.0", "bigFont.fnt");
				tch->setPositionY(301);
				tch->setPositionX(200);
				tch->setScale(.375f);
				tch->setAnchorPoint({});

				addChild(tch);
				return true;
			}

			#if __APPLE__
			bool init()
			{
				return inithook();
			}
			#endif
		};
		#else
		bool (*init_o)(MenuLayer *);

		bool init(MenuLayer *self)
		{
			if (!init_o(self))
				return false;

			auto tch = CCLabelBMFont::create("TechnoGDPS RELEASE 1.0", "bigFont.fnt");
			tch->setPositionY(301);
			tch->setPositionX(200);
			tch->setScale(.375f);
			tch->setAnchorPoint({});

			self->addChild(tch);

			return true;
		}
		#endif
		void applyHooks() {
			#ifndef __ANDROID__
			auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
			MH_CreateHook(
				reinterpret_cast<void *>(base + 0x1907b0),
				reinterpret_cast<void *>(extract(&MyMenuLayer::inithook)),
				reinterpret_cast<void **>(&MyMenuLayer::_init)
			);
			#else
			HOOK_FUNC("_ZN9MenuLayer4initEv");
			#endif
		}
	}
	namespace TLevelTools {


		#ifdef __ANDROID__
		void* (*_getLevel)(LevelTools *self, int, bool);

		void* getLevel(LevelTools *self, int lvl_id, bool a1) {
			switch(lvl_id) {
				case 22: {
					auto level = GJGameLevel::create();
					level->levelName =  "Time Machine 2";
					level->difficulty = (4);
					level->audioTrack = (19);
					level->stars      = (12);
					return level;
				}
			}
			return _getLevel(self, lvl_id, a1);
		}
		#endif
		void applyHooks() {
			int arch = 0;
			
			#ifdef __arm__
			arch = 1;
			#endif
			#ifdef __i386
			arch = 2;
			#endif

			#ifdef __ANDROID__
			HOOK_FUNCX("_ZN10LevelTools8getLevelEib", getLevel, _getLevel);
			PatchManager *ss = PatchManager::getSharedState();

			switch(arch) {
				case 1: {
					//ss->patch(AS_ADDRESS(0x0022cbbc), 0x17);
					// ss->patch(AS_ADDRESS(0x0022cc34), 0x17);
					// ss->patch(AS_ADDRESS(0x0022f5c4), 0x17);
					// ss->patch(AS_ADDRESS(0x0022fab2), 0x17);
					ss->patch(AS_ADDRESS(0x002d0f1a), 0x17);
					// ss->patch(AS_ADDRESS(0x002d3df0), 0x17);
					break;
				}
				case 2: {
					ss->patch(AS_ADDRESS(0x002624e5), 0x17);
					ss->patch(AS_ADDRESS(0x00262625), 0x17);
					ss->patch(AS_ADDRESS(0x00262565), 0x17);
					ss->patch(AS_ADDRESS(0x00268c32), 0x17);
					ss->patch(AS_ADDRESS(0x002694e9), 0x17);
					ss->patch(AS_ADDRESS(0x0026941d), 0x17);
					ss->patch(AS_ADDRESS(0x003aefda), 0x17);
					ss->patch(AS_ADDRESS(0x003b4a11), 0x17);
					break;
				}
			}
			// ss->patch()
			#endif
		}
	}
	namespace TGameObject {
		void applyHooks() {

		}
	}
	namespace TCreatorLayer {
		#define CHECKPOS(x, y) node2->getPositionX() == x && node2->getPositionY() == y

		#ifndef __ANDROID__
		bool __fastcall init(CCLayer* self) {
			if(!Cvolton::MHook::getOriginal(init)(self)) return false;

			CCObject *obj = NULL;
			CCNode* node = NULL;
			CCNode *node_to_remove = NULL;
			CCARRAY_FOREACH(self->getChildren(), obj) {
				node = (CCNode *)obj;
				if(node->getPositionX() == 284.5f && node->getPositionY() == 160.f) {
					CCObject *obj2 = NULL;
					CCNode *node2 = NULL;
					CCARRAY_FOREACH(node->getChildren(), obj2) {
						node2 = (CCNode *)obj2;
						if(CHECKPOS(-100.f, 97.f)) { // 0
							node2->setPositionX(-110.f);
							node2->setPositionY(97.f);	
						}
						if(CHECKPOS(0.f, 97.f)) { // 1
							node2->setPositionX(-1.f);
						}
						if(CHECKPOS(100.f, 97.f)) { // 2
							node2->setPositionX(109.f);
						}
						if(CHECKPOS(-150.f, 0.f)) { // 3
							node2->setPositionX(-160.f);
						}
						if(CHECKPOS(-50.f, 0.f)) { // 4
							node2->setPositionX(-52.f);
						}
						if(CHECKPOS(50.f, 0.f)) { // 5
							node_to_remove = node2;
						}
						if(CHECKPOS(-150.f, -97.f)) { // 6
							node2->setPositionX(-113.f);
						}
						if(CHECKPOS(-50.f, -97.f)) { // 7
							node2->setPositionX(-1.f);
						}
						if(CHECKPOS(50.f, -97.f)) { // 8
							node2->setPositionX(56.f);
							node2->setPositionY(0.f);
						}
						if(CHECKPOS(150.f, -97.f)) { // 9
							node2->setPositionX(112.f);
						}
						if(CHECKPOS(150.f, 0.f)) { // 10
							node2->setPositionX(165.f);
						}
					}
				}
			}

			node_to_remove->removeMeAndCleanup();

			return true;
		}
		#endif
		void applyHooks() {
			#ifndef __ANDROID__
			Cvolton::MHook::registerHook(base + 0x4DE40, init);
			#endif
		}
	}	
	void applyHooks()
	{
		Techno::TMenuLayer::applyHooks();
		Techno::TLevelTools::applyHooks();
		Techno::TGameObject::applyHooks();
		Techno::TCreatorLayer::applyHooks();
	}
}

void inject()
{
#if _WIN32
	Techno::applyHooks();
	MH_EnableHook(MH_ALL_HOOKS);
#endif
}

#if _WIN32
WIN32CAC_ENTRY(inject)
#endif

#ifdef __ANDROID__
__attribute__((constructor)) void libinit()
{
	PatchManager::getSharedState()->init();
	Techno::applyHooks();
}
#endif
