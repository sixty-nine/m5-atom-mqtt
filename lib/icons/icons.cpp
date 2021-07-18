#include <chrono>
#include <thread>
#include "icons.h"

namespace sixtynine
{
    using namespace std::this_thread;
    using namespace std::chrono;

    // ----- Display ------------------------------------------------------------------------

    Display::Display(LED_DisPlay *display)
    {
        _display = display;
        _xSemaphore = xSemaphoreCreateMutex();
    }

    void Display::showFrame(Frame *frame)
    {
        displayBuffer(frame->buffer);
        sleep_for(milliseconds { frame->duration });
    }

    void Display::showAnimation(Frames *frames)
    {
        for(Frame frame : *frames) {
            showFrame(&frame);
        }
    }

    void Display::displayBuffer(void * buff)
    {
        xSemaphoreTake(_xSemaphore, portMAX_DELAY);
        _display->clear();
        _display->displaybuff((uint8_t *)buff, 0, 0);
        xSemaphoreGive(_xSemaphore);
    }

    // ----- Animation ----------------------------------------------------------------------

    Animation::Animation()
    {
        _frames = new Frames();
    }

    Animation::~Animation()
    {
        delete _frames;
    }

    void Animation::addFrame(Frame *frame)
    {
        _frames->push_back(*frame);
    }

    Frames *Animation::getFrames()
    {
        return _frames;
    }

    // --------------------------------------------------------------------------------------

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
