#ifndef _D3D12_SWAP_CHAIN_HPP_
#define _D3D12_SWAP_CHAIN_HPP_

#include "Graphics_deprecated/SwapChain.hpp"

struct IDXGISwapChain;
struct IDXGISwapChain3;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Window;
		class CommandQueue;
		class Resource;
		class D3D12Resource;
		class D3D12DescriptorHeap;
		class D3D12View;
		class D3D12RenderPass;

		class D3D12SwapChain : public SwapChain
		{
		public:
			D3D12SwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj);
			virtual ~D3D12SwapChain();

			virtual void Present() override;

			//virtual void AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext) override;
			//virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext) override;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;
			virtual RenderPass& GetMainRenderPass() override;

			IDXGISwapChain* GetDXGISwapChain() { return _SwapChain; }

		private:
			vector<D3D12Resource*>			_BackBuffers;
			vector<D3D12View*>				_BackBufferViews;
			vector< vector<View*> >			_BackBufferViewReferences;
			vector< vector<BlendState> >	_BlendStateReferences;
			vector<D3D12RenderPass*>		_RenderPasses;
			
			D3D12DescriptorHeap*			_BackBufferDescriptorHeap	= nullptr;
			IDXGISwapChain*					_SwapChain					= nullptr;
			IDXGISwapChain3*				_SwapChain3					= nullptr;
			uint32_t						_BackBuffersCount			= 0u;
		};
	}
}

#endif
