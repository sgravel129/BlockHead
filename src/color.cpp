#include "color.hpp"

#include <stdlib.h>

Color::Color(const std::string &hexcode)
{
    int hex = (int)strtol(hexcode.c_str(), NULL, 16);
    _red = ((hex >> 16) & 0xFF);
    _green = ((hex >> 8) & 0xFF);
    _blue = (hex & 0xFF);
    _alpha = 255;
}