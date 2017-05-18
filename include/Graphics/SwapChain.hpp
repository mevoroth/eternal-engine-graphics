#ifndef _SWAP_CHAIN_HPP_
#define _SWAP_CHAIN_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;
		class CommandQueue;
		class Resource;
		class View;
		class RenderPass;

		class SwapChain
		{
		public:
			virtual ~SwapChain() {}
			virtual uint32_t AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj) = 0;
			virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex) = 0;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) = 0;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) = 0;
			virtual uint32_t GetBackBuffersFrameCount() const = 0;
			virtual RenderPass& GetMainRenderPass() = 0;
		};
	}
}

#endif
