#include "d3d12/D3D12Renderer.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>

using namespace Eternal::Graphics;

#ifdef ETERNAL_DEBUG
#define ETERNAL_D3D12_DXGIFLAG	(0x1)
#else
#define ETERNAL_D3D12_DXGIFLAG	(0x0)
#endif

D3D12Renderer::D3D12Renderer()
{
	HRESULT hr = CreateDXGIFactory2(ETERNAL_D3D12_DXGIFLAG, __uuidof(IDXGIFactory4), (void**)&_DXGIFactory);
	ETERNAL_ASSERT(hr == S_OK);
	
#ifdef ETERNAL_DEBUG
	// Enable the D3D12 debug layer
	hr = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&_Debug);
	if (hr == S_OK)
	{
		_Debug->EnableDebugLayer();
	}
#endif

	IDXGIAdapter1* DXGIAdapter = nullptr;

	for (UINT adapterIndex = 0; adapterIndex < MAX_DXGI_ADAPTER; ++adapterIndex)
	{
		if (_DXGIFactory->EnumAdapters1(adapterIndex, &DXGIAdapter) == DXGI_ERROR_NOT_FOUND)
		{
			DXGIAdapter = nullptr;
			break;
		}

		hr = D3D12CreateDevice(DXGIAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)&_Device);
		ETERNAL_ASSERT(hr == S_OK);
		ETERNAL_ASSERT(_Device);

		_DXGIAdapter = DXGIAdapter;
	}

	ETERNAL_ASSERT(_DXGIAdapter);
}
