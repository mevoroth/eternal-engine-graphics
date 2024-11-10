#pragma once

#if ETERNAL_PLATFORM_WINDOWS

#include <windows.h>
#include <d3d12.h>
#include <DXGI1_4.h>
#include "d3dx12.h"

#if ETERNAL_USE_PIX
#include "WinPixEventRuntime/pix3.h"
#endif

#if ETERNAL_USE_DEBUG_LAYER
#include <dxgidebug.h>
#endif

#else // ETERNAL_PLATFORM_WINDOWS
#include "d3d12/D3D12LibraryPrivate.h"
#endif // ETERNAL_PLATFORM_WINDOWS
