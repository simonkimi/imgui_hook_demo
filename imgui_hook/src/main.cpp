#include <pch.h>
#include "logger/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef RENDER_API_OPENGL
#include "../imgui/impl/imgui_opengl_impl.h"
#endif

#ifdef RENDER_API_DX11
#include "imgui/imgui_d311_impl.h"
#endif


void OnProcessAttach();

void OnProcessDetach();

void InitLogger();

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD fdw_reason, LPVOID lpv_reserved)
{
    switch (fdw_reason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(h_instance);
            InitLogger();
            OnProcessAttach();
            break;
        case DLL_PROCESS_DETACH:
            OnProcessDetach();
            break;
        default:
            break;
    }
    return TRUE;
}

void OnProcessAttach()
{
#ifdef RENDER_API_OPENGL
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) ImGuiOpenGLImpl::StartHook, nullptr, 0, nullptr);
#elif defined(RENDER_API_DX11)
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) ImguiD311Impl::StartHook, nullptr, 0, nullptr);
#endif
}

void OnProcessDetach()
{
#ifdef RENDER_API_OPENGL
    ImGuiOpenGLImpl::EndHook();
#elif defined(RENDER_API_DX11)
    ImguiD311Impl::EndHook();
#endif
}

void InitLogger()
{
    auto logger = spdlog::stdout_color_mt("console");
    auto debug_sink = std::make_shared<output_debug_string_sink<std::mutex>>();
    logger->sinks().push_back(debug_sink);
    logger->set_level(spdlog::level::debug);
    logger->flush_on(spdlog::level::debug);
    spdlog::set_pattern("[%l] [%H:%M:%S] %v (%s:%#)");
    spdlog::set_default_logger(logger);
}