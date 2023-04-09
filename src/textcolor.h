#ifndef COLORTEXT
#define COLORTEXT

#include <iostream>

namespace utils {
    namespace color {
        enum ColorCode {
            FG_DEFAULT = 39,
            FG_BLACK = 30,
            FG_RED = 31,
            FG_GREEN = 32,
            FG_YELLOW = 33,
            FG_BLUE = 34,
            FG_MAGENTA = 35,
            FG_CYAN = 36,
            FG_WHITE = 37,

            BG_DEFAULT = 49,
            BG_BLACK = 40,
            BG_RED = 41,
            BG_GREEN = 42,
            BG_YELLOW = 43,
            BG_BLUE = 44,
            BG_MANGETA = 45,
            BG_CYAN = 46,
            BG_WHITE = 47
        };
    }

    class TextColor {
            color::ColorCode code;

        public:
            TextColor(color::ColorCode pCode) { code = pCode; }
            friend std::ostream& operator << (std::ostream& os, TextColor& textColor){
                return os << "\033[" << textColor.code << "m";
            }
    };
}
#endif