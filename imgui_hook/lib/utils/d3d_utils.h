#pragma once

#include "pch.h"

#ifndef HR
#define HR(x) { HRESULT hr = (x); if (FAILED(hr)) { DxTrace(hr); } }
#endif

#ifndef RELEASE_COM
#define RELEASE_COM(x) { if (x) { x->Release(); x = nullptr; } }
#endif


void DxTrace(HRESULT hresult);


void GetDx11VTable(HWND hwnd, void **v_table, int size);
