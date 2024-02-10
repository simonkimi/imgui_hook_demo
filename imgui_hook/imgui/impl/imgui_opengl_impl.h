#pragma once

#include "imgui_core.h"
#include "backends/imgui_impl_opengl3_loader.h"

using FuncWglSwapBuffer = BOOL(WINAPI *)(HDC hDc);
using FuncGlBegin = void (WINAPI *)(GLenum mode);
using FuncGlClear = void (WINAPI *)(GLbitfield mask);
using FuncGlColor4f = void (WINAPI *)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

class ImGuiOpenGLImpl : public ImguiCore {
private:
    static FuncWglSwapBuffer vfun_wgl_swap_buffer_;
    static bool is_opengl2_;

public:
    static void StartHook();

    static void EndHook();
    
private:
    static void InitImgui();

    static BOOL WINAPI HookWglSwapBuffer(HDC hdc);
};