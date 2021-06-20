#include "d3d12_deprecated/D3D12SwapChain.hpp"

#include <cwchar>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "Window/Window.hpp"
#include "Graphics/Format.hpp"
#include "d3d12_deprecated/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12CommandQueue.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"
#include "d3d12_deprecated/D3D12Resource.hpp"
#include "d3d12_deprecated/D3D12View.hpp"
#include "d3d12_deprecated/D3D12RenderPass.hpp"
#include "NextGenGraphics/Context.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/RenderPassFactory.hpp"

using namespace Eternal::Graphics;

D3D12SwapChain::D3D12SwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj)
	: SwapChain(WindowObj)
	, _BackBuffersCount(2)
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
	SwapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

	HRESULT hr = D3D12Device::GetDXGIFactory()->CreateSwapChain(static_cast<D3D12CommandQueue&>(CommandQueueObj).GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain);
	ETERNAL_ASSERT(hr == S_OK);

	hr = _SwapChain->QueryInterface(&_SwapChain3);
	ETERNAL_ASSERT(hr == S_OK);

	//hr = _SwapChain3->GetMaximumFrameLatency(&_BackBuffersCount);
	//ETERNAL_ASSERT(hr == S_OK);

	//hr = _DXGIFactory->MakeWindowAssociation(WindowObj.GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER);
	//ETERNAL_ASSERT(hr == S_OK);

	vector<RootSignatureParameter> Parameters;
	Parameters.resize(GetBackBuffersFrameCount());
	Parameters[0] = { RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RENDER_TARGET, RootSignatureAccess::ROOT_SIGNATURE_ACCESS_IA_VS_PS, 0 };
	Parameters[1] = { RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RENDER_TARGET, RootSignatureAccess::ROOT_SIGNATURE_ACCESS_IA_VS_PS, 0 };

	_BackBufferDescriptorHeap = new D3D12DescriptorHeap(DeviceObj, Parameters.data(), uint32_t(Parameters.size()));
	_BackBuffers.resize(GetBackBuffersFrameCount());
	_BackBufferViews.resize(GetBackBuffersFrameCount());
	_BackBufferViewReferences.resize(GetBackBuffersFrameCount());
	_RenderPasses.resize(GetBackBuffersFrameCount());
	_BlendStateReferences.resize(GetBackBuffersFrameCount());

	wchar_t BackBufferName[256];

	for (uint32_t BackBufferFrameIndex = 0; BackBufferFrameIndex < GetBackBuffersFrameCount(); ++BackBufferFrameIndex)
	{
		ID3D12Resource* BackBuffer = nullptr;
		hr = _SwapChain->GetBuffer(BackBufferFrameIndex, __uuidof(ID3D12Resource), (void**)&BackBuffer);
		ETERNAL_ASSERT(hr == S_OK);
		swprintf(BackBufferName, ETERNAL_ARRAYSIZE(BackBufferName), L"Back Buffer %d", BackBufferFrameIndex);
		BackBuffer->SetName(BackBufferName);
		_BackBuffers[BackBufferFrameIndex]		= new D3D12Resource(BackBuffer);
		_BackBufferViews[BackBufferFrameIndex]	= static_cast<D3D12View*>(_BackBuffers[BackBufferFrameIndex]->CreateRenderTargetView(DeviceObj, *_BackBufferDescriptorHeap, FORMAT_RGBA8888));

		_BackBufferViewReferences[BackBufferFrameIndex].push_back(_BackBufferViews[BackBufferFrameIndex]);
		_BlendStateReferences[BackBufferFrameIndex].push_back(GetBackBufferBlendState());

		RenderPassCreateInformation RenderPassInformation(
			GetMainViewport(),
			_BackBufferViewReferences[BackBufferFrameIndex],
			_BlendStateReferences[BackBufferFrameIndex]
		);
		//_RenderPasses[BackBufferFrameIndex]		= static_cast<D3D12RenderPass*>(CreateRenderPass(RenderPassInformation));
	}
}

D3D12SwapChain::~D3D12SwapChain()
{
	for (uint32_t BackBufferFrameIndex = 0; BackBufferFrameIndex < GetBackBuffersFrameCount(); ++BackBufferFrameIndex)
	{
		delete _BackBufferViews[BackBufferFrameIndex];
		_BackBufferViews[BackBufferFrameIndex] = nullptr;
		delete _BackBuffers[BackBufferFrameIndex];
		_BackBuffers[BackBufferFrameIndex] = nullptr;
	}
	delete _BackBufferDescriptorHeap;
	_BackBufferDescriptorHeap = nullptr;
	_SwapChain->Release();
	_SwapChain = nullptr;
}

void D3D12SwapChain::Present()
{
	ETERNAL_BREAK();
}

//void D3D12SwapChain::AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext)
//{
//	UINT FrameIndex = _SwapChain3->GetCurrentBackBufferIndex();
//	GfxContext.SetFrameIndex(FrameIndex);
//	GfxContext.SetFrameRenderPass(_RenderPasses[FrameIndex]);
//	GfxContext.SetFrameBackBuffer(_BackBuffers[FrameIndex]);
//}

//void D3D12SwapChain::Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext)
//{
//	HRESULT hr = _SwapChain->Present(0, 0);
//	if (hr != S_OK)
//	{
//		ETERNAL_BREAK();
//		hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetDeviceRemovedReason();
//		ETERNAL_BREAK();
//	}
//}

//Resource& D3D12SwapChain::GetBackBuffer(_In_ uint32_t BackBufferIndex)
//{
//	return *_BackBuffers[BackBufferIndex];
//}
//
//View& D3D12SwapChain::GetBackBufferView(_In_ uint32_t BackBufferIndex)
//{
//	return *_BackBufferViews[BackBufferIndex];
//}

uint32_t D3D12SwapChain::GetBackBuffersFrameCount() const
{
	return _BackBuffersCount;
}

RenderPass& D3D12SwapChain::GetMainRenderPass()
{
	return *_RenderPasses[0];
}
