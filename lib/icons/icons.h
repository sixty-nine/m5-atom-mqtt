#ifndef icons_h
#define icons_h

#include "M5Atom.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace sixtynine
{
    struct RGB
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    struct Frame
    {
        unsigned long duration;
        unsigned char buffer[77];
    };

    typedef std::vector<Frame> Frames;

    void copyIcon(
        const unsigned char source[],
        unsigned char dest[],
        RGB fgColor,
        RGB bgColor = { 0, 0, 0 }
    );

    class Animation
    {
        public:
            Animation();
            ~Animation();
            void addFrame(Frame *frame);
            Frames *getFrames();
        private:
            Frames *_frames;
    };

    class Display
    {
        public:
            Display(LED_DisPlay *display);
            void showFrame(Frame *frame);
            void showAnimation(Frames *frames);
            void displayBuffer(void * buff);
        private:
            LED_DisPlay *_display;
            SemaphoreHandle_t _xSemaphore = NULL;
    };

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


        const unsigned char press1[25] = {
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
        };

        const unsigned char press2[25] = {
            0, 0, 0, 0, 0,
            0, 1, 1, 1, 0,
            0, 1, 0, 1, 0,
            0, 1, 1, 1, 0,
            0, 0, 0, 0, 0,
        };

        const unsigned char press3[25] = {
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 0, 0, 0,
            0, 0, 0, 0, 0,
        };
    }

    namespace animations {

        class PressAnimation: public Animation
        {
            public:
                PressAnimation() : Animation()
                {
                    Frame frame1 = { .duration = 50 };
                    Frame frame2 = { .duration = 50 };
                    Frame frame3 = { .duration = 50 };

                    copyIcon(icons::press3, frame1.buffer, { 0x45, 0x8b, 0xd1 });
                    copyIcon(icons::press2, frame2.buffer, { 0x32, 0x65, 0x98 });
                    copyIcon(icons::press1, frame3.buffer, { 0x23, 0x46, 0x6a });

                    addFrame(&frame1);
                    addFrame(&frame2);
                    addFrame(&frame3);
                }
        };

        class NetworkConnectAnimation: public Animation
        {
            public:
                NetworkConnectAnimation() : Animation()
                {
                    Frame frame1 = { .duration = 500 };
                    Frame frame2 = { .duration = 500 };
                    Frame frame3 = { .duration = 1000 };

                    copyIcon(icons::network1, frame1.buffer, { 0x45, 0x8b, 0xd1 });
                    copyIcon(icons::network2, frame2.buffer, { 0x32, 0x65, 0x98 });
                    copyIcon(icons::network3, frame3.buffer, { 0x23, 0x46, 0x6a });

                    addFrame(&frame1);
                    addFrame(&frame2);
                    addFrame(&frame3);
                }
        };

    }

}

#endif
