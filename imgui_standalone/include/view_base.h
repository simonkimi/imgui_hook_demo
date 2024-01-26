#pragma once


class IView {
    virtual bool Render(HWND hwnd) = 0;
};