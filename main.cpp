#include "App.hpp"
#include "Win.hpp"

#include <signal.h>

using namespace std;
using namespace mobo;

void sigHandler(int iSig)
{
    if(App::shared) {
        App::shared->alive = false;
    }
}

int main(int argc, char**argv)
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sigHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    App app(argc, argv);
    Win* newWin = app.newWindow(100, 100, 2048, 1024);

    Renderer* rend = new Renderer();
    newWin->setRenderer(rend);

    app.run();

    delete rend;
    
    return 0;
}
