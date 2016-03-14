#include "d3d12/D3D12Device.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>

#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "Window/Window.hpp"

using namespace Eternal::Graphics;

#ifdef ETERNAL_DEBUG
#define ETERNAL_D3D12_DXGIFLAG	(0x1)
#else
#define ETERNAL_D3D12_DXGIFLAG	(0x0)
#endif

D3D12Device::D3D12Device()
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

	_CreateDirectCommandQueue();
}

void D3D12Device::CreateSwapChain(Window& WindowObj)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	
	SwapChainDesc.BufferDesc.Width = WindowObj.GetWidth();
	SwapChainDesc.BufferDesc.Height = WindowObj.GetHeight();
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = WindowObj.GetWindowHandler();
	SwapChainDesc.Windowed = WindowObj.GetWindowed() ? TRUE : FALSE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = _DXGIFactory->CreateSwapChain(_CommandQueue->GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain);
	ETERNAL_ASSERT(hr == S_OK);

	_BackBuffer = new D3D12RenderTarget(*this);
}

void D3D12Device::_CreateDirectCommandQueue()
{
	_CommandQueue = new D3D12CommandQueue(*this);
}
