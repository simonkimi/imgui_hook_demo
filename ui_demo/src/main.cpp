#include "../include/main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    auto controller = new ImguiController();
    auto view = new MainView(controller->GetHwnd());
    while (true) {
        if (!controller->Loop([&view] { return view->Render(); })) {
            break;
        }
    }
    delete view;
    delete controller;
    return 0;
}
