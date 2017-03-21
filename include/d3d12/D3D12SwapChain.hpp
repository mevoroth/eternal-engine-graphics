#ifndef _D3D12_SWAP_CHAIN_HPP_
#define _D3D12_SWAP_CHAIN_HPP_

#include "Graphics/SwapChain.hpp"

struct IDXGISwapChain;
struct IDXGISwapChain3;

#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Window;
		class CommandQueue;
		class RenderTarget;
		class D3D12RenderTarget;
		class D3D12DescriptorHeap;

		class D3D12SwapChain : public SwapChain
		{
		public:
			D3D12SwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj);

			virtual uint32_t AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj) override;
			virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex) override;

			virtual RenderTarget& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;

			IDXGISwapChain* GetDXGISwapChain() { return _SwapChain; }

		private:
			vector<D3D12RenderTarget*> _BackBuffers;
			
			D3D12DescriptorHeap*	_BackBufferDescriptorHeap	= nullptr;
			IDXGISwapChain*			_SwapChain					= nullptr;
			IDXGISwapChain3*		_SwapChain3					= nullptr;
		};
	}
}

#endif
