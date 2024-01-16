#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    auto controller = new ImguiController();
    auto view = new MainView();
    while (controller->loop([view] { return view->render(); }));
    delete view;
    delete controller;
    return 0;
}
