#define CAC_PROJ_NAME "Template"

#include "techno.h"
#include "LevelTools.h"
#include "CreatorLayer.h"
#include "InfoLayer.h"
#include "networking.h"
#include "Rainix/LC/main.h"
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
	namespace TKeysLayer {
		#ifndef __ANDROID__
		bool __thiscall init(CCLayer *self) {
			if(!Cvolton::MHook::getOriginal(init)(self)) return false;

			self->sortAllChildren();
			auto shop1_00 = (CCNode *)(self->getChildren()->objectAtIndex(3));
			shop1_00->sortAllChildren();
			auto shop1_01 = (CCNode *)(shop1_00->getChildren()->objectAtIndex(0));
			shop1_01->sortAllChildren();
			auto shop1_02 = (CCNode *)(shop1_01->getChildren()->objectAtIndex(22));
			shop1_02->sortAllChildren();
			auto shop1_03 = (CCNode *)(shop1_02->getChildren()->objectAtIndex(0));
			shop1_03->sortAllChildren();
			auto shop1_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(0));
			auto shop2_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(1));

			shop1_04->setPositionX(0.f);
			shop2_04->removeMeAndCleanup();

			return true;
		}
		#else
		bool (*_init)(CCLayer *self);

		bool init(CCLayer *self) {
			if(!_init(self)) return false;

			self->sortAllChildren();
			auto shop1_00 = (CCNode *)(self->getChildren()->objectAtIndex(3));
			shop1_00->sortAllChildren();
			auto shop1_01 = (CCNode *)(shop1_00->getChildren()->objectAtIndex(0));
			shop1_01->sortAllChildren();
			auto shop1_02 = (CCNode *)(shop1_01->getChildren()->objectAtIndex(22));
			shop1_02->sortAllChildren();
			auto shop1_03 = (CCNode *)(shop1_02->getChildren()->objectAtIndex(0));
			shop1_03->sortAllChildren();
			auto shop1_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(0));
			auto shop2_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(1));

			shop1_04->setPositionX(0.f);
			shop2_04->removeMeAndCleanup();

			return true;
		}
		#endif
		void applyHooks() {
			#ifndef __ANDROID__
			Cvolton::MHook::registerHook(base + 0x154560, init);
			#else
			HOOK_FUNCX("_ZN9KeysLayer4initEv", init, _init);
			#endif
		}
	}
	namespace TInfoLayer {
		#ifndef __ANDROID__
		void __fastcall onComment(InfoLayer *self, CCObject *sender) {
			// GJAccountManager *ss = GJAccountManager::sharedState();
			// int a = ss->getAccountID();
			// if(a == 0) { // unregistered
			// 	ss->setAccountID(65535);
			// }
			// Cvolton::MHook::getOriginal(onComment)(self, sender);
			// //ss->setAccountID(a);

			// return;
		}
		#endif
		void applyHooks() {
			#ifndef __ANDROID__
			// Cvolton::MHook::registerHook(base + 0x151600, onComment);
			#endif
		}
	}
	namespace TMessage {
		void test(CCLayer *l) {
			#ifndef __ANDROID__
			// DialogObject *dobj = GJDialogObject::create("123", "456", 5, 1.f, false, {100, 100, 100});
			// l->addChild((CCNode *)dobj);
			#endif
		}
	}
	namespace TMenuLayer {
		class MenuLayer_TestAction : public FLAlertLayerProtocol {
		private:
			CCObject *pTarget;
		public:
			void setTarget(CCObject *target) {
				pTarget = target;
			}
	
			void FLAlert_Clicked(FLAlertLayer *alertlayer, bool btn2) {
				SimpleHTTPRequestLayer *l = (SimpleHTTPRequestLayer *)pTarget;

				l->close();

				alertlayer->removeMeAndCleanup();
			};
		};

		#ifndef __ANDROID__

		class $implement(MenuLayer, MyMenuLayer) {
		public:
			static inline bool(__thiscall * _init)(MenuLayer * self);
			static inline void(__thiscall * _onPlay)(CCObject *sender);

			static void buttonCallback(CCObject * sender) {
				MenuLayer_TestAction *action = new MenuLayer_TestAction;

				action->setTarget(sender);

				auto alert = FLAlertLayer::create(action, "Error", "Ok", NULL, "New levels will be added after <cg>CC!</c>");
				alert->show();
			}

			void nCallback(CCHttpClient* client, CCHttpResponse* response) {
				// SimpleHTTPRequestLayer *l = (SimpleHTTPRequestLayer *)(response->getHttpRequest()->getTarget());

				// l->close();
				MyMenuLayer::buttonCallback(response->getHttpRequest()->getTarget());

				return;
			}

			void onPlay(CCObject *sender) {
				// _onPlay(sender);
				// return;

				SimpleHTTPRequestLayer *l = SimpleHTTPRequestLayer::create();
				l->start("https://gd.dogotrigger.xyz/tech21/getOfficialLevels21.php", httpresponse_selector(MyMenuLayer::nCallback));

				CCNode *cn = (CCNode *)sender;

				cn->addChild(l, 1024);

				l->m_pLC->setPosition(cn->getPositionX(), cn->getPositionY());
				l->setPosition(56, 56);
			}

			bool inithook() {
				printf("inithook\n");

				if (!_init(this))
					return false;

				// GJAccountManager *ss = GJAccountManager::sharedState();
				// int a = ss->getAccountID();
				// if(a == 0) { // unregistered
				// 	ss->setAccountID(65535);
				// }

				TMessage::test((CCLayer *)this);

				CCSize winSize = CCDirector::sharedDirector()->getWinSize();

				auto tch = CCLabelBMFont::create("TechnoGDPS BETA BUILD 1.0", "bigFont.fnt");
				tch->setPositionY(300);
				tch->setPositionX(winSize.width / 2);
				tch->setScale(.375f);
				tch->setAnchorPoint({0.5f, 0.f});

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

		class MenuLayer_IOActions {
		public:
			static void buttonCallback(CCObject * sender) {
				MenuLayer_TestAction *action = new MenuLayer_TestAction;

				CCLayer *cl = (CCLayer *)sender;

				cl->removeMeAndCleanup();
			}
			void nCallback(CCHttpClient* client, CCHttpResponse* response) {
				// SimpleHTTPRequestLayer *l = (SimpleHTTPRequestLayer *)(response->getHttpRequest()->getTarget());

				// l->close();
				MenuLayer_IOActions::buttonCallback(response->getHttpRequest()->getTarget());

				return;
			}
			void onPlay(CCObject *sender) {
				SimpleHTTPRequestLayer *l = SimpleHTTPRequestLayer::create();
				l->start("https://gd.dogotrigger.xyz/tech21/getOfficialLevels21.php", httpresponse_selector(MenuLayer_IOActions::nCallback));

				CCNode *cn = (CCNode *)sender;

				cn->addChild(l, 1024);

				l->m_pLC->setPosition(cn->getPositionX(), cn->getPositionY());
				l->setPosition(56, 56);
			}
		};

		bool init(MenuLayer *self)
		{
			if (!init_o(self))
				return false;

			CCSize winSize = CCDirector::sharedDirector()->getWinSize();

			auto tch = CCLabelBMFont::create("TechnoGDPS BETA BUILD 1.0", "bigFont.fnt");
			tch->setPositionY(300);
			tch->setPositionX(winSize.width / 2);
			tch->setScale(.375f);
			tch->setAnchorPoint({0.5f, 0.f});

			self->addChild(tch);

			self->sortAllChildren();
			// auto shop1_00 = (CCNode *)(self->getChildren()->objectAtIndex(3));
			// shop1_00->sortAllChildren();
			// auto shop1_01 = (CCNode *)(shop1_00->getChildren()->objectAtIndex(0));
			// shop1_01->sortAllChildren();
			// auto shop1_02 = (CCNode *)(shop1_01->getChildren()->objectAtIndex(22));
			// shop1_02->sortAllChildren();
			// auto shop1_03 = (CCNode *)(shop1_02->getChildren()->objectAtIndex(0));
			// shop1_03->sortAllChildren();
			// auto shop1_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(0));
			// auto shop2_04 = (CCNode *)(shop1_03->getChildren()->objectAtIndex(1));

			// shop1_04->setPositionX(0.f);
			// shop2_04->removeMeAndCleanup();
			// auto pb00 = (CCNode *)(self->getChildren()->objectAtIndex(2));
			// pb00->sortAllChildren();
			// auto pb01 = (CCNode *)(self->getChildren()->objectAtIndex(0));

			// pb01->removeMeAndCleanup();

			// CCMenu *men = CCMenu::create();
			// CCSprite* PlaySprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
			// CCMenuItemSpriteExtra *Play = CCMenuItemSpriteExtra::create(
				// PlaySprite,
				// PlaySprite,
				// PlaySprite,
				// menu_selector(MenuLayer_IOActions::onPlay)
			// );
			// men->addChild(Play);
			// pb00->addChild(men);

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
			// MH_CreateHook(
				// reinterpret_cast<void *>(base + 0x191b50),
				// reinterpret_cast<void *>(extract(&MyMenuLayer::onPlay)),
				// reinterpret_cast<void **>(&MyMenuLayer::_onPlay)
			// );
			#else
			HOOK_FUNC("_ZN9MenuLayer4initEv");
			// HOOK_FUNCX("_ZN9MenuLayer6onPlayEPN7cocos2d8CCObject", onPlay, _onPlay);
			#endif
		}
	}
	namespace TLevelTools {
		void applyHooks() {
			// int arch = 0;
			
			// #ifdef __arm__
			// arch = 1;
			// #endif
			// #ifdef __i386
			// arch = 2;
			// #endif

			// #ifdef __ANDROID__
			// HOOK_FUNCX("_ZN10LevelTools8getLevelEib", getLevel, _getLevel);
			// PatchManager *ss = PatchManager::getSharedState();

			// switch(arch) {
			// 	case 1: {
			// 		ss->patch(AS_ADDRESS(0x0022cbbc), 0x17);
			// 		ss->patch(AS_ADDRESS(0x0022cc34), 0x17);
			// 		ss->patch(AS_ADDRESS(0x0022f5c4), 0x17);
			// 		ss->patch(AS_ADDRESS(0x0022fab2), 0x17);
			// 		ss->patch(AS_ADDRESS(0x002d0f1a), 0x17);
			// 		ss->patch(AS_ADDRESS(0x002d3df0), 0x17);
			// 		break;
			// 	}
			// 	case 2: {
			// 		ss->patch(AS_ADDRESS(0x002624e5), 0x17);
			// 		ss->patch(AS_ADDRESS(0x00262625), 0x17);
			// 		ss->patch(AS_ADDRESS(0x00262565), 0x17);
			// 		ss->patch(AS_ADDRESS(0x00268c32), 0x17);
			// 		ss->patch(AS_ADDRESS(0x002694e9), 0x17);
			// 		ss->patch(AS_ADDRESS(0x0026941d), 0x17);
			// 		ss->patch(AS_ADDRESS(0x003aefda), 0x17);
			// 		ss->patch(AS_ADDRESS(0x003b4a11), 0x17);
			// 		break;
			// 	}
			// }
			// // ss->patch()
			// #endif
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
			CCNode *node_to_remove = CCNode::create();

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
							if(node_to_remove != NULL) {
								node_to_remove->removeMeAndCleanup();
							}
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
		#else
		bool (*_init)(CCLayer *self);

		bool init(CCLayer *self) {
			if(!_init(self)) return false;

			CCObject *obj = NULL;
			CCNode* node = NULL;
			CCNode *node_to_remove = CCNode::create();
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
							if(node_to_remove != NULL) {
								node_to_remove->removeMeAndCleanup();
							}
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
			#else
			HOOK_FUNCX("_ZN12CreatorLayer4initEv", init, _init);
			#endif
		}
	}	
	void applyHooks()
	{
		Techno::TMenuLayer::applyHooks();
		Techno::TLevelTools::applyHooks();
		Techno::TGameObject::applyHooks();
		Techno::TCreatorLayer::applyHooks();
		Techno::TInfoLayer::applyHooks();
		Techno::TKeysLayer::applyHooks();
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
	//PatchManager::getSharedState()->init();
	Techno::applyHooks();
	Rainix_LCLoader();
}
#endif
