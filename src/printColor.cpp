#include "printColor.hpp"

void printColor(const char *text, int color, int style, int style2)
{
    printf("\033[%i;%i;%im%s\033[0m", color, style, style2, text);
}
