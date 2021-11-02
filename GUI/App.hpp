#ifndef __APP_HPP__
#define __APP_HPP__

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#include "EventHandler.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <unordered_map>
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace mobo
{
    class Win;

    typedef unordered_map<int, Win*> WinMap;

    class Win;

    class App : public EventHandler
    {
        public:
            static App* shared;

        public:
            App(int, char**);
            virtual ~App();

            virtual bool idle();

            bool run();
            void tick();
            
            virtual bool keyDown(unsigned char k, int x, int y);
            virtual bool specialUp(int k, int x, int y);

            Win* newWindow(int x, int y, int w, int h);
            void removeWindow(int);

            WinMap windows;

            bool alive;

            void yieldToUI();
            time_point<steady_clock> yieldToUITime;

    };
}

#endif
