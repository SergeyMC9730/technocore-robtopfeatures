#define CAC_PROJ_NAME "Template"

#if __APPLE__
	#include <CacKit>
	using namespace cocos2d;
#else
	#include "win32cac.h"
#endif

#include "LevelTools.h"

class $implement(LevelTools, MyLevelTools) {
public:
	static inline GJGameLevel *(__thiscall *_getLevel)(LevelTools *self, int lid, bool p1);
	static inline const char*  (__thiscall *_getAudioTitle)(LevelTools *self, int aid);
	static inline int		   (__thiscall *_getArtistForAudio)(LevelTools *self, int aid);
	static inline const char* (__thiscall *_getURLForAudio)(LevelTools *self, int aid);
	static inline const char*  (__thiscall *_getAudioFilename)(LevelTools *self, int aid);
	static inline bool         (__thiscall *_verifyLevelIntegrity)(LevelTools *self, int lid);

	const char *getAudioTitleHook(int aid) {
		printf("getAudioTitleHook %d\n", aid);
		switch(aid) {
			case 22: {
				return "Time Machine 2";
				break;
			}
			default: {
				return _getAudioTitle(this, aid);
			}
		}
	}

	int getArtistForAudio(int aid) {
		printf("getArtistForAudio %d\n", aid);
		switch(aid) {
			case 21: {
				return 1;
				break;
			}
			default: {
				return _getArtistForAudio(this, aid);
			}
		}
	}

	// https://www.youtube.com/watch?v=awCuqziUpFA
	const char* getURLForAudio(int aid) {
		printf("getURLForAudio %d\n", aid);
		switch(aid) {
			case 22: {
				return "https://www.youtube.com/watch?v=awCuqziUpFA";
				break;
			}
			default: {
				return _getURLForAudio(this, aid);
			}
		}
	}
	
	const char* getAudioFilename(int aid) {
		printf("getAudioFilename %d\n", aid);
		switch(aid) {
			case 22: {
				return "TimeMachine2.mp3";
				break;
			}
			default: {
				return _getAudioFilename(this, aid);
			}
		}
	}

	bool verifyLevelIntegrity(int lid) {
		printf("verifyLevelIntegrity %d\n", lid);
		return true;
	}

	GJGameLevel *getLevelHook(int lid, bool p1) {
		printf("getLevelHook %d %d\n", lid, p1);
		switch(lid) {
			case 22: {
				GJGameLevel *lvl = GJGameLevel::create();
				int sid = lid - 1;

				lvl->m_nLevelID = lid;
				lvl->m_sLevelName = "Time Machine 2";
				lvl->m_nAudioTrack = sid;
				lvl->m_nStars = 13;
				lvl->m_nDifficulty = 5;
				lvl->m_nCoins = 3;
				lvl->m_eLevelType = GJLevelType::kGJLevelTypeLocal;
				lvl->m_nOrbCompletion = 325;

				return lvl;

				break;
			}
			default: {
				GJGameLevel *l = _getLevel(this, lid, p1);
				std::cout << "- " << l->m_sLevelName << "\n";
				return l;
			}
		}
	}
};

class $implement(MenuLayer, MyMenuLayer) {
 public:
	static inline bool (__thiscall* _init)(MenuLayer* self);

	void buttonCallback(CCObject* sender) {
		auto alert = FLAlertLayer::create(NULL, "Mod", "Ok", NULL, "<cg>custom button!</c>");
		alert->show();
	}

	bool inithook() {
		printf("inithook\n");
		
		if (!_init(this)) return false;

		auto tch = CCLabelBMFont::create("TechnoGDPS RELEASE 1.0", "bigFont.fnt");
		tch->setPositionY(301);
		tch->setPositionX(200);
		tch->setScale(.375f);
		tch->setAnchorPoint({});

		addChild(tch);
		return true;
	}

	#if __APPLE__
	bool init() {return inithook();}
	#endif
};

void inject() {
	#if _WIN32
	auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
	
	MH_CreateHook(
	    reinterpret_cast<void*>(base + 0x1907b0),
		reinterpret_cast<void*>(extract(&MyMenuLayer::inithook)),
	    reinterpret_cast<void**>(&MyMenuLayer::_init)
	);
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x189370),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::getLevelHook)),
	//     reinterpret_cast<void**>(&MyLevelTools::_getLevel)
	// );
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x189C60),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::getAudioTitleHook)),
	//     reinterpret_cast<void**>(&MyLevelTools::_getAudioTitle)
	// );
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x18A8C0),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::getURLForAudio)),
	//     reinterpret_cast<void**>(&MyLevelTools::_getURLForAudio)
	// );
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x18A2D0),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::getArtistForAudio)),
	//     reinterpret_cast<void**>(&MyLevelTools::_getArtistForAudio)
	// );
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x189FA0),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::getAudioFilename)),
	//     reinterpret_cast<void**>(&MyLevelTools::_getAudioFilename)
	// );
	// MH_CreateHook(
	//     reinterpret_cast<void*>(base + 0x18B180),
	// 	reinterpret_cast<void*>(extract(&MyLevelTools::verifyLevelIntegrity)),
	//     reinterpret_cast<void**>(&MyLevelTools::_verifyLevelIntegrity)
	// );
	MH_EnableHook(MH_ALL_HOOKS);
	#endif
}

#if _WIN32
	WIN32CAC_ENTRY(inject)
#endif
