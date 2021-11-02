#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#include "MovingAverage.hpp"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

namespace mobo
{
    class Renderer
    {
        public:
            Renderer() : timestamp(time_point<steady_clock>(seconds(0))), emaWindow(0.0), fpsWMA(0.0), fpsEMA(0.0), fpsAVG(30), prog(0) { }
            virtual ~Renderer();

            void didReshape(int w, int h);

            virtual void init();
            virtual void render();
            virtual float calculateFrameRate();

        protected:
            time_point<steady_clock> timestamp;
            float emaWindow;
            float fpsWMA;
            float fpsEMA;
            MovingAverage fpsAVG;
            GLint prog;
            GLuint vbo[3];
            GLuint tex;
            GLuint samp;

    };
}

#endif
