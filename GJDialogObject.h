#include "techno.h"

#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>

// OH GOD
#ifndef __ANDROID
class GJDialogObject : public DialogObject {
public:
    static DialogObject *create(std::string const& a, std::string const& b, int c, float d, bool l, cocos2d::_ccColor3B const& color) {
        #if _WIN32
        auto aCopy = a;
        auto bCopy = b;
        union {
          struct {
              int t1;
              int t2;
              int t3;
              int t4;
              int t5;
              int t6;
          } t;
          std::array<char, sizeof(std::string)> s;
        } au, bu;
        std::memcpy(&au, &aCopy, sizeof(std::string));
        std::memcpy(&bu, &bCopy, sizeof(std::string));
        union {
          struct {
              int t1;
          } t;
          cocos2d::_ccColor3B s;
        } su;
        su.s = color;
        auto ret = reinterpret_cast<DialogObject *(__vectorcall *)(int, bool, int, int, int, int, int, int, int, int, int, int, int, int, int, float, float, float, float)>(
            base + 0x6D160
        )(c, l, au.t.t1,au.t.t2,au.t.t3,au.t.t4,au.t.t5,au.t.t6, bu.t.t1,au.t.t2,bu.t.t3,bu.t.t4,bu.t.t5,bu.t.t6, su.t.t1, 8008.5, 8008.5, 8008.5, d);
        assert(ret);

        __asm add esp, 0x34;

        return ret;
        #else
        return nullptr;
        #endif
    }
};

#endif
