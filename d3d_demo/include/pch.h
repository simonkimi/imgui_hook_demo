#pragma once

#include <Windows.h>
#include <tchar.h>
#include <comdef.h>
#include <d3d11.h>
#include <iostream>
#include <wrl/client.h>

#ifndef RELEASE_COM
#define RELEASE_COM(x) { if(x){ x->Release(); x = nullptr; } }
#endif

using Microsoft::WRL::ComPtr;