#include "App.hpp"
#include "Win.hpp"

#include <iostream>
#include <thread>

using namespace std;

namespace mobo
{
    void _idle(void* iAppPtr)
    {
        cout << "WTF?!?\n";
        App* app = (App*) iAppPtr;
        app->dispatchEvent(Event(Event::IDLE));
    }

    void _tick(int, void* iAppPtr)
    {
        App* app = (App*) iAppPtr;
        app->tick();
        glutTimerFuncUcall(30, _tick, 0, iAppPtr);
    }

    App* App::shared = nullptr;

    App::App(int argc, char**argv)
    : yieldToUITime(steady_clock::now() - seconds(10))
    {
        shared = this;

        glutInit(&argc, argv); 
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowSize(2048, 1024);
        glutInitWindowPosition(100, 100);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        glutTimerFuncUcall(30, _tick, 0, (void*) this);
    }

    App::~App()
    {
        glutIdleFuncUcall(nullptr, nullptr);

        while(windows.size()) {
            auto i = windows.begin();
            Win* w = i->second;
            windows.erase(i);
            delete w;
        }
    }

    bool App::idle()
    {
        return true;
    }

    Win* App::newWindow(int x, int y, int w, int h)
    {
        glutInitWindowSize(w, h);
        glutInitWindowPosition(x, y);
        Win* newWin = new Win(*this); windows[newWin->winId] = newWin;
        return newWin;
    }

    void App::removeWindow(int iWinId)
    {
        unordered_map<int, Win*>::iterator i = windows.find(iWinId);
        if(i != windows.end()) {
            Win *win = i->second;
            win->winId = 0;
            windows.erase(i);
            delete win;
        }
        if(windows.size() == 0) {
            alive = false;
        }
    }

    void App::yieldToUI()
    {
        yieldToUITime = steady_clock::now();
    }

    bool App::run()
    {

        alive = true;
        while(alive) {
            glutMainLoopEvent();
            auto currentTime = steady_clock::now();
            if((currentTime - yieldToUITime) < seconds(3)) {
                this_thread::sleep_for(milliseconds(60));
            }
        }
    }

    void App::tick()
    {
        for(auto i : windows) {
            i.second->postDisplay();
        }
    }

    bool App::keyDown(unsigned char k, int x, int y)
    {
        switch(k) {
            case 17: // ctrl-Q
                alive = false;
                break;
            default:
                cout << (int) k << endl;
        }
        return true;
    }

    bool App::specialUp(int k, int x, int y)
    {
        return true;
    }

}
