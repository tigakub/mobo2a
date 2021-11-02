#include "Win.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace mobo
{
    void _didMove(int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::DIDMOVE, 0, x, y));
    }

    void _didReshape(int w, int h)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::DIDRESHAPE, 0, w, h));
    }

    void _willClose()
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::WILLCLOSE));
    }

    void _keyDown(unsigned char k, int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::KEYDOWN, k, x, y));
    }

    void _keyUp(unsigned char k, int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::KEYUP, k, x, y));
    }

    void _specialDown(int k, int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::SPECIALDOWN, k, x, y));
    }

    void _specialUp(int k, int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::SPECIALUP, k, x, y));
    }

    void _mouseMove(int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::MOUSEMOVE, 0, x, y));
    }

    void _mouse(int b, int d, int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        uint32_t button = 0;
        switch(b) {
            case GLUT_LEFT_BUTTON:
                button = Win::PRIMARY_BUTTON; break;
            case GLUT_RIGHT_BUTTON:
                button = Win::SECONDARY_BUTTON; break;
            case GLUT_MIDDLE_BUTTON:
                button = Win::TERTIARY_BUTTON; break;
            default: break;
        }
        d ? ((win->mouseButtonState |= button) && win->dispatchEvent(Event(Event::MOUSEDOWN, button, x, y)))
        : ((win->mouseButtonState &= ~button) && win->dispatchEvent(Event(Event::MOUSEUP, button, x, y)));
    }

    void _mouseDrag(int x, int y)
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::MOUSEDRAG, 0, x, y));
    }

    void _mouseSense(int d)
    {
        Win* win = (Win*) glutGetWindowData();
        switch(d) {
            case GLUT_ENTERED:
                win->dispatchEvent(Event(Event::MOUSEENTERED)); break;
            case GLUT_LEFT:
                win->dispatchEvent(Event(Event::MOUSEEXITED)); break;
            default: break;
        }
    }

    void _render()
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::RENDER));
    }

    void _renderOverlay()
    {
        Win* win = (Win*) glutGetWindowData();
        win->dispatchEvent(Event(Event::RENDEROVERLAY)); 
    }

    Win::Win(App& iApp)
    : app(iApp), winId(glutCreateWindow("")),
      mouseButtonState(0), renderer(nullptr), needsDisplay(false)
    {
        glutSetWindow(winId);
        glutSetWindowData((void*) this);

        glutPositionFunc(_didMove);
        glutReshapeFunc(_didReshape);
        glutCloseFunc(_willClose);

        glutKeyboardFunc(_keyDown);
        glutKeyboardUpFunc(_keyUp);

        glutSpecialFunc(_specialDown);
        glutSpecialUpFunc(_specialUp);

        glutPassiveMotionFunc(_mouseMove);
        glutMouseFunc(_mouse);
        
        glutMotionFunc(_mouseDrag);

        glutEntryFunc(_mouseSense);

        glutDisplayFunc(_render);
        glutOverlayDisplayFunc(_renderOverlay);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(1.0f);
    }

    Win::~Win()
    {
        if(renderer) {
            renderer = nullptr;
        }

        if(winId) {
            glutSetWindow(winId);
            glutSetWindowData(nullptr);

            glutPositionFunc(nullptr);
            glutReshapeFunc(nullptr);
            glutCloseFunc(nullptr);

            glutKeyboardFunc(nullptr);
            glutKeyboardUpFunc(nullptr);

            glutSpecialFunc(nullptr);
            glutSpecialUpFunc(nullptr);

            glutPassiveMotionFunc(nullptr);
            glutMouseFunc(nullptr);
            
            glutMotionFunc(nullptr);

            glutEntryFunc(nullptr);

            glutDestroyWindow(winId);
        }
        winId = 0;
    }

    void Win::setTitle(const string& iTitle)
    {
        glutSetWindow(winId);
        glutSetWindowTitle(iTitle.c_str());
    }

    bool Win::didReshape(int w, int h)
    {
        if(renderer) {
            renderer->didReshape(w, h);
        }
        app.yieldToUI();
        return true;
    }

    bool Win::willClose()
    {
        app.removeWindow(winId);
        return true;
    }

    bool Win::keyDown(unsigned char k, int x, int y)
    {
        switch(k) {
            case 14: // ctrl-N
                return true;
                /*
                {
                    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
                    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
                    int winx = glutGet(GLUT_WINDOW_X);
                    int winy = glutGet(GLUT_WINDOW_Y);
                    int winw = glutGet(GLUT_WINDOW_WIDTH);
                    int winh = glutGet(GLUT_WINDOW_HEIGHT);
                    winx += 20;
                    winy += 10;
                    if((winx + winw + 20) > screenWidth) { winx = 20; }
                    if((winy + winh + 20) > screenHeight) { winy = 20; }
                    app.newWindow(winx, winy, winw, winh);
                } return true;
                */
            case 23: // ctrl-W
                glutDestroyWindow(winId);
                return true;
        }
        return false;
    }

    bool Win::keyUp(unsigned char k, int x, int y)
    {
        return false;
    }

    void Win::postDisplay()
    {
        if(!needsDisplay) {
            glutSetWindow(winId);
            glutPostRedisplay();
            needsDisplay = true;
        }
    }

    bool Win::render()
    {
        if(renderer) {
            renderer->render();
            float fpsEMA = renderer->calculateFrameRate();
            stringstream fpsSStream;
            fpsSStream << "FPS: " << setprecision(2) << fpsEMA;
            setTitle(fpsSStream.str());
        } else {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glutSwapBuffers();
        }
        needsDisplay = false;
        return true;
    }

}
