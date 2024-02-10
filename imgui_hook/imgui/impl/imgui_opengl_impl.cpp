#include "imgui_opengl_impl.h"
#include "process_helper.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl2.h"
#include "detours/detours.h"

FuncWglSwapBuffer ImGuiOpenGLImpl::vfun_wgl_swap_buffer_ = nullptr;
bool ImGuiOpenGLImpl::is_opengl2_ = false;


void ImGuiOpenGLImpl::StartHook()
{
    HMODULE h_module = GetModuleHandle(L"opengl32.dll");
    if (!h_module) {
        SPDLOG_ERROR("Failed to get opengl32.dll");
        return;
    }

    vfun_wgl_swap_buffer_ = (FuncWglSwapBuffer) GetProcAddress(h_module, "wglSwapBuffers");
    if (!vfun_wgl_swap_buffer_) {
        SPDLOG_ERROR("Failed to get wglSwapBuffers");
        return;
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) vfun_wgl_swap_buffer_, HookWglSwapBuffer);
    DetourTransactionCommit();
}

void ImGuiOpenGLImpl::EndHook()
{
    if (is_initialized_) {
        is_initialized_ = false;
        UnhookWndProc();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) vfun_wgl_swap_buffer_, HookWglSwapBuffer);
    DetourTransactionCommit();
}

void ImGuiOpenGLImpl::InitImgui()
{
    SPDLOG_INFO("ImGuiOpenGLImpl::InitImgui()");

    // 判断opengl版本
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    SPDLOG_INFO("OpenGL Version: {}.{}", major, minor);
    is_opengl2_ = major * 10 + minor < 32;
    HookWndProc();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    ImGui_ImplWin32_InitForOpenGL(hwnd_);
    if (is_opengl2_) {
        ImGui_ImplOpenGL2_Init();
    } else {
        ImGui_ImplOpenGL3_Init();
    }

    is_initialized_ = true;
}

BOOL ImGuiOpenGLImpl::HookWglSwapBuffer(HDC hdc)
{
    if (!hwnd_)
        hwnd_ = WindowFromDC(hdc);
    if (!is_initialized_)
        InitImgui();

    if (is_opengl2_)
        ImGui_ImplOpenGL2_NewFrame();
    else
        ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplWin32_NewFrame();
    DrawFrame();
    if (is_opengl2_)
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    else
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return vfun_wgl_swap_buffer_(hdc);
}



