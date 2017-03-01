#include "d3d12/D3D12Device.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>

#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "Window/Window.hpp"

using namespace Eternal::Graphics;

#ifdef ETERNAL_DEBUG
#define ETERNAL_D3D12_DXGIFLAG	(0x1)
#else
#define ETERNAL_D3D12_DXGIFLAG	(0x0)
#endif

ID3D12Debug* D3D12Device::_Debug = nullptr;
IDXGIFactory4* D3D12Device::_DXGIFactory = nullptr;

void D3D12Device::Initialize()
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

}

D3D12Device::D3D12Device(_In_ uint32_t DeviceIndex)
{
	ETERNAL_ASSERT(_DXGIFactory);
	HRESULT hr = _DXGIFactory->EnumAdapters1(DeviceIndex, &_DXGIAdapter);
	
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		// No GPU at this index
		return;
	}

	hr = D3D12CreateDevice(_DXGIAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)&_Device);
	ETERNAL_ASSERT(hr == S_OK);
	ETERNAL_ASSERT(_Device);

#ifdef ETERNAL_DEBUG
	DXGI_ADAPTER_DESC DXGIAdapterDesc;
	hr = _DXGIAdapter->GetDesc(&DXGIAdapterDesc);
	ETERNAL_ASSERT(hr == S_OK); // Break here for debug info on device
#endif

	_DeviceMask = 1 << DeviceIndex;

	_CreateDirectCommandQueue();
}

void D3D12Device::CreateSwapChain(_In_ const Window& WindowObj)
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
	SwapChainDesc.BufferCount = GetBackBufferFrameCount();
	SwapChainDesc.OutputWindow = WindowObj.GetWindowHandler();
	SwapChainDesc.Windowed = WindowObj.GetWindowed() ? TRUE : FALSE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = _DXGIFactory->CreateSwapChain(_CommandQueue->GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain);
	ETERNAL_ASSERT(hr == S_OK);

	hr = _SwapChain->QueryInterface(&_SwapChain3);
	ETERNAL_ASSERT(hr == S_OK);

	//hr = _DXGIFactory->MakeWindowAssociation(WindowObj.GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER);
	//ETERNAL_ASSERT(hr == S_OK);

	_BackBufferDescriptorHeap = new D3D12DescriptorHeap(*this, D3D12DescriptorHeap::RENDERTARGET, GetBackBufferFrameCount());
	_BackBuffers = new D3D12RenderTarget*[GetBackBufferFrameCount()];

	for (uint32_t BackBufferFrameIndex = 0; BackBufferFrameIndex < GetBackBufferFrameCount(); ++BackBufferFrameIndex)
	{
		_BackBuffers[BackBufferFrameIndex] = new D3D12RenderTarget(*this, *_BackBufferDescriptorHeap, BackBufferFrameIndex);
	}
}

uint32_t D3D12Device::GetDeviceMask() const
{
	ETERNAL_ASSERT(_DeviceMask != 0xFFFFFFFF);
	return _DeviceMask;
}

D3D12RenderTarget*const & D3D12Device::GetBackBuffer(_In_ uint32_t BackBufferIndex)
{
	ETERNAL_ASSERT(BackBufferIndex < GetBackBufferFrameCount());
	return _BackBuffers[BackBufferIndex];
}

void D3D12Device::_CreateDirectCommandQueue()
{
	_CommandQueue = new D3D12CommandQueue(*this);
}
