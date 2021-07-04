#include "icons.h"

namespace sixtynine
{
    void copyIcon(
        const unsigned char source[],
        unsigned char dest[],
        RGB fgColor,
        RGB bgColor
    ) {
        memset(dest, 0, 77);

        dest[0] = 5;
        dest[1] = 5;
        int curpos = 2;

        struct RGB curColor = { 0, 0, 0 };

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (source[i * 5 + j]) {
                    curColor = { fgColor.r, fgColor.g, fgColor.b };
                } else {
                    curColor = { bgColor.r, bgColor.g, bgColor.b };
                }

                dest[curpos] = curColor.r;
                dest[curpos + 1] = curColor.g;
                dest[curpos + 2] = curColor.b;

                curpos += 3;
            }
        }
    }

}
