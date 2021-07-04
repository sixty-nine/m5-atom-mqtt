#ifndef icons_h
#define icons_h

#include "M5Atom.h"

namespace growlab
{
    struct RGB
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    void copyIcon(
        const unsigned char source[],
        unsigned char dest[],
        RGB fgColor,
        RGB bgColor = { 0, 0, 0 }
    );

    namespace icons
    {
        const unsigned char bBlank[5] = {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
        };

        const unsigned char blank[25] = {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        };

        const unsigned char ok[25] = {
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
        };

        const unsigned char network1[25] = {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            1, 0, 0, 0, 0,
        };


        const unsigned char network2[25] = {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            1, 1, 0, 0, 0,
            0, 0, 1, 0, 0,
            1, 0, 1, 0, 0,
        };


        const unsigned char network3[25] = {
            1, 1, 1, 0, 0,
            0, 0, 0, 1, 0,
            1, 1, 0, 0, 1,
            0, 0, 1, 0, 1,
            1, 0, 1, 0, 1,
        };

        const unsigned char up[25] = {
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
        };

        const unsigned char mqtt[25] = {
            0, 0, 1, 0, 0,
            0, 1, 1, 0, 0,
            1, 1, 1, 1, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
        };

    }
}

#endif
