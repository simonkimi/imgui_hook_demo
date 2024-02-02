#include <pch.h>
#include "d3d_hook.h"
#include "logger/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>


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
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) D3d11Hook::StartHook, nullptr, 0, nullptr);
}

void OnProcessDetach()
{
    D3d11Hook::EndHook();
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