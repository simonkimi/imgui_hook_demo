#pragma once

#include "Windows.h"
#include "tchar.h"
#include "imgui.h"
#include <wrl/client.h>
#include <comdef.h>
#include "spdlog/spdlog.h"

using Microsoft::WRL::ComPtr;



#ifndef RELEASE_COM
#define RELEASE_COM(x) { if (x) { x->Release(); x = nullptr; } }
#endif