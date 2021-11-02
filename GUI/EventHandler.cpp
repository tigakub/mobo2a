#include "EventHandler.hpp"

namespace mobo
{
    bool EventHandler::dispatchEvent(const Event &e)
    {
        switch(e.id) {
            case Event::IDLE:
                return idle();
            case Event::DIDMOVE:
                return didMove(e.x, e.y);
            case Event::DIDRESHAPE:
                return didReshape(e.x, e.y);
            case Event::WILLCLOSE:
                return willClose();
            case Event::KEYDOWN:
                return keyDown(e.k, e.x, e.y);
            case Event::KEYUP:
                return keyUp(e.k, e.x, e.y);
            case Event::SPECIALDOWN:
                return specialDown(e.k, e.x, e.y);
            case Event::SPECIALUP:
                return specialUp(e.k, e.x, e.y);
            case Event::MOUSEMOVE:
                return mouseMove(e.x, e.y);
            case Event::MOUSEDOWN:
                return mouseDown(e.k, e.x, e.y);
            case Event::MOUSEUP:
                return mouseUp(e.k, e.x, e.y);
            case Event::MOUSEDRAG:
                return mouseDrag(e.x, e.y);
            case Event::MOUSEENTERED:
                return mouseEntered();
            case Event::MOUSEEXITED:
                return mouseExited();
            case Event::RENDER:
                return render();
            case Event::RENDEROVERLAY:
                return renderOverlay();
        }
    }
}
