#include "d3d12/D3D12WindowsSwapChain.hpp"

#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS

#include <cwchar>
#include "Graphics/Format.hpp"
#include "d3d12/D3D12Library.h"
#include "d3d12/D3D12WindowsDevice.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Graphics/View.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Viewport.hpp"
#include "Windows/WindowsOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12WindowsSwapChain::D3D12WindowsSwapChain(_In_ GraphicsContext& InContext)
			: SwapChain()
			, _BackBuffersCount(GraphicsContext::FrameBufferingCount)
		{
			using namespace Eternal::Graphics::D3D12;

			WindowsOutputDevice& InOutputDevice = static_cast<WindowsOutputDevice&>(InContext.GetOutputDevice());

			DXGI_SWAP_CHAIN_DESC SwapChainDesc;

			SwapChainDesc.BufferDesc.Width						= InOutputDevice.GetWidth();
			SwapChainDesc.BufferDesc.Height						= InOutputDevice.GetHeight();
			SwapChainDesc.BufferDesc.RefreshRate.Numerator		= InOutputDevice.GetVSync() ? 0 : 60;
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
			SwapChainDesc.OutputWindow							= InOutputDevice.GetWindowHandler();
			SwapChainDesc.Windowed								= InOutputDevice.GetWindowed() ? TRUE : FALSE;
			SwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			SwapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT
																| (InOutputDevice.GetVSync() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : DXGI_SWAP_CHAIN_FLAG(0));

			VerifySuccess(
				D3D12WindowsDevice::GetDXGIFactory()->CreateSwapChain(static_cast<D3D12CommandQueue&>(InContext.GetGraphicsQueue()).GetD3D12CommandQueue(), &SwapChainDesc, &_SwapChain)
			);

			VerifySuccess(
				_SwapChain->QueryInterface(&_SwapChain3)
			);
	
			VerifySuccess(
				_SwapChain3->SetMaximumFrameLatency(_BackBuffersCount)
			);
			//hr = _SwapChain3->GetMaximumFrameLatency(&_BackBuffersCount);
			//ETERNAL_ASSERT(hr == S_OK);

			//hr = _DXGIFactory->MakeWindowAssociation(InWindow.GetWindowHandler(), DXGI_MWA_NO_ALT_ENTER);
			//ETERNAL_ASSERT(hr == S_OK);

			memset(&SwapChainDesc, 0x0, sizeof(DXGI_SWAP_CHAIN_DESC));
			VerifySuccess(_SwapChain->GetDesc(&SwapChainDesc));
			ETERNAL_ASSERT(SwapChainDesc.BufferCount == _BackBuffersCount);

			_BackBuffers.resize(_BackBuffersCount);
			_BackBufferRenderTargetViews.resize(_BackBuffersCount);
			for (uint32_t BackBufferIndex = 0; BackBufferIndex < _BackBuffersCount; ++BackBufferIndex)
			{
				ID3D12Resource* BackBufferResource = nullptr;

				VerifySuccess(
					_SwapChain->GetBuffer(
						BackBufferIndex,
						__uuidof(ID3D12Resource),
						reinterpret_cast<void**>(&BackBufferResource)
					)
				);

				ETERNAL_ASSERT(BackBufferResource);

				// Resource
				{
					std::string BackBufferName = "BackBuffer" + std::to_string(BackBufferIndex);
					D3D12ResourceBackBufferCreateInformation CreateInformation(
						InContext.GetDevice(),
						BackBufferName,
						BackBufferResource
					);

					_BackBuffers[BackBufferIndex] = new D3D12Resource(CreateInformation);
				}

				// View
				{
					ViewMetaData MetaData;
					RenderTargetViewCreateInformation CreateInformation(
						InContext,
						_BackBuffers[BackBufferIndex],
						MetaData,
						Format::FORMAT_RGBA8888_UNORM,
						ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D
					);
					_BackBufferRenderTargetViews[BackBufferIndex] = new D3D12View(CreateInformation);
				}
			}
		}

		D3D12WindowsSwapChain::~D3D12WindowsSwapChain()
		{
			for (uint32_t BackBufferIndex = 0; BackBufferIndex < _BackBuffersCount; ++BackBufferIndex)
			{
				delete _BackBufferRenderTargetViews[BackBufferIndex];
				_BackBufferRenderTargetViews[BackBufferIndex] = nullptr;

				delete _BackBuffers[BackBufferIndex];
				_BackBuffers[BackBufferIndex] = nullptr;
			}

			_SwapChain->Release();
			_SwapChain = nullptr;
		}

		void D3D12WindowsSwapChain::Acquire(_In_ GraphicsContext& InContext)
		{
			InContext.GetCurrentFrameIndex() = _SwapChain3->GetCurrentBackBufferIndex();
		}

		void D3D12WindowsSwapChain::Present(_In_ GraphicsContext& InContext)
		{
			using namespace Eternal::Graphics::D3D12;

			HRESULT HResult = _SwapChain->Present(0, 0);

#if ETERNAL_USE_NVIDIA_AFTERMATH
			static_cast<D3D12Device&>(InContext.GetDevice()).GetNVIDIANsightAftermath().OnPresent(HResult);
#endif

			VerifySuccess(InContext.GetDevice(), HResult);

			static_cast<D3D12Fence&>(InContext.GetCurrentFrameFence()).Signal(
				static_cast<D3D12CommandQueue&>(InContext.GetGraphicsQueue())
			);
		}
	}
}

#endif
