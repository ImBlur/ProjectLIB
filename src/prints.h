#ifndef PRINTS
#define PRINTS

#include <iostream>
#include "textcolor.h"

namespace utils {
    void error(const char* message){
        utils::TextColor red(utils::color::FG_RED);
        utils::TextColor def(utils::color::FG_DEFAULT);

        std::cout << red << "[ERROR]: " << message << def << "\n";
    }

    void warning(const char* message){
        utils::TextColor yellow(utils::color::FG_YELLOW);
        utils::TextColor def(utils::color::FG_DEFAULT);

        std::cout << yellow << "[WARNING]: " << message << def << "\n";
    }
}

#endif