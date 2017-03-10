#ifndef _D3D12_SWAP_CHAIN_HPP_
#define _D3D12_SWAP_CHAIN_HPP_

#include "Graphics/SwapChain.hpp"

struct IDXGISwapChain;
struct IDXGISwapChain3;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		class CommandQueue;
		class FrameBuffer;

		class D3D12SwapChain : public SwapChain
		{
		public:
			D3D12SwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj);

			virtual uint32_t AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj) override;
			virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex) override;

			virtual FrameBuffer& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;

		private:
			IDXGISwapChain* _SwapChain		= nullptr;
			IDXGISwapChain3* _SwapChain3	= nullptr;
		};
	}
}

#endif
