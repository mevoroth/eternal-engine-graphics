#include "d3d12/D3D12SwapChain.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <dxgi1_4.h>
#include "Window/Window.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "d3d12/D3D12RenderTarget.hpp"

using namespace Eternal::Graphics;

D3D12SwapChain::D3D12SwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	SwapChainDesc.BufferDesc.Width						= WindowObj.GetWidth();
	SwapChainDesc.BufferDesc.Height						= WindowObj.GetHeight();
	SwapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	SwapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	SwapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	SwapChainDesc.SampleDesc.Count						= 1;
	SwapChainDesc.SampleDesc.Quality					= 0;
	SwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount							= GetBackBuffersFrameCount();
	SwapChainDesc.OutputWindow							= WindowObj.GetWindowHandler();
	SwapChainDesc.Windowed								= WindowObj.GetWindowed() ? TRUE : FALSE;
	SwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SwapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D12Device::GetDXGIFactory()->CreateSwapChain(static_cast<D3D12CommandQueue&>(CommandQueueObj).GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain);
	ETERNAL_ASSERT(hr == S_OK);

	hr = _SwapChain->QueryInterface(&_SwapChain3);
	ETERNAL_ASSERT(hr == S_OK);

	//hr = _DXGIFactory->MakeWindowAssociation(WindowObj.GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER);
	//ETERNAL_ASSERT(hr == S_OK);

	_BackBufferDescriptorHeap = new D3D12DescriptorHeap(DeviceObj, D3D12DescriptorHeap::RENDERTARGET, GetBackBuffersFrameCount());
	_BackBuffers.resize(GetBackBuffersFrameCount());
	for (uint32_t BackBufferFrameIndex = 0; BackBufferFrameIndex < GetBackBuffersFrameCount(); ++BackBufferFrameIndex)
	{
		_BackBuffers[BackBufferFrameIndex] = new D3D12RenderTarget(DeviceObj, *this, *_BackBufferDescriptorHeap, BackBufferFrameIndex);
	}
}

uint32_t D3D12SwapChain::AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj)
{
	return _SwapChain3->GetCurrentBackBufferIndex();
}

void D3D12SwapChain::Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex)
{
	HRESULT hr = _SwapChain->Present(0, 0);
	ETERNAL_ASSERT(hr == S_OK);
}

RenderTarget& D3D12SwapChain::GetBackBuffer(_In_ uint32_t BackBufferIndex)
{
	return *_BackBuffers[BackBufferIndex];
}

uint32_t D3D12SwapChain::GetBackBuffersFrameCount() const
{
	return 2;
}
