#include "d3d12/D3D12SwapChain.hpp"

#include <cwchar>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "Window/Window.hpp"
#include "Graphics_deprecated/Format.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"
#include "d3d12_deprecated/D3D12Resource.hpp"
#include "d3d12_deprecated/D3D12View.hpp"
#include "d3d12_deprecated/D3D12RenderPass.hpp"
#include "NextGenGraphics/Context.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics_deprecated/RenderPassFactory.hpp"

using namespace Eternal::Graphics;

D3D12SwapChain::D3D12SwapChain(_In_ GraphicsContext& Context)
	: SwapChain(Context.GetWindow())
	, _BackBuffersCount(GraphicsContext::FrameBufferingCount)
{
	Window& WindowObj = Context.GetWindow();

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
	SwapChainDesc.BufferCount							= _BackBuffersCount;
	SwapChainDesc.OutputWindow							= WindowObj.GetWindowHandler();
	SwapChainDesc.Windowed								= WindowObj.GetWindowed() ? TRUE : FALSE;
	SwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SwapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

	HRESULT hr = D3D12Device::GetDXGIFactory()->CreateSwapChain(static_cast<D3D12CommandQueue&>(Context.GetGraphicsQueue()).GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain);
	ETERNAL_ASSERT(hr == S_OK);

	hr = _SwapChain->QueryInterface(&_SwapChain3);
	ETERNAL_ASSERT(hr == S_OK);

	//hr = _SwapChain3->GetMaximumFrameLatency(&_BackBuffersCount);
	//ETERNAL_ASSERT(hr == S_OK);

	//hr = _DXGIFactory->MakeWindowAssociation(WindowObj.GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER);
	//ETERNAL_ASSERT(hr == S_OK);
}

D3D12SwapChain::~D3D12SwapChain()
{
	_SwapChain->Release();
	_SwapChain = nullptr;
}

void D3D12SwapChain::Present()
{
	ETERNAL_BREAK();
}
