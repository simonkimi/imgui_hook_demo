#include "../include/main.h"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    auto controller = std::make_unique<ImguiController>();
    auto view = std::make_unique<MainView>(controller->GetHwnd());
    while (true) {
        if (!controller->Loop([&view] { return view->Render(); })) {
            break;
        }
    }
    
    return 0;
}
