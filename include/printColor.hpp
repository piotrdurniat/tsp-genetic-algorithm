#ifndef PRINT_COLOR_H
#define PRINT_COLOR_H

#include <iostream>

enum TextColor : int
{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    BROWN = 33,
    BLUE = 34,
    PURPLE = 35,
    CYAN = 36,
    LIGHT_GRAY = 37
};

enum TextStyle : int
{
    NORMAL = 0,
    BOLD = 1,
    UNDERLINED = 4,
    BLINKING = 5,
    REVERSE = 7,
};

void printColor(const char *text, int color, int style, int style2);

#endif
