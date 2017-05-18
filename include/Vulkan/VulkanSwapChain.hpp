#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#include <vector>
#include <cstdint>

#include "Graphics/SwapChain.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window;
		class Device;
		class CommandQueue;
		class Resource;
		class View;
		class VulkanView;
		class VulkanRenderPass;
		class VulkanFence;
		class VulkanResource;

		class VulkanSwapChain : public SwapChain
		{
		public:
			VulkanSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj);

			virtual uint32_t AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj) override;
			virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex) override;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;
			virtual RenderPass& GetMainRenderPass() override;
			VkSwapchainKHR_T*& GetSwapChain() { return _SwapChain; }

			VkSemaphore_T*& GetAcquireSemaphore(_In_ uint32_t ResourceIndex);

		private:
			Viewport				_BackBufferViewport;
			BlendState				_BackBufferBlendState;
			vector<VulkanResource*>	_BackBuffers;
			vector<View*>			_BackBufferViews;
			vector<VkSemaphore_T*>	_AcquireSemaphores;
			VulkanRenderPass*		_RenderPass	= nullptr;
			VkSurfaceKHR_T*			_Surface	= nullptr;
			VkSwapchainKHR_T*		_SwapChain	= nullptr;
			uint32_t				_FrameIndex	= 0u;
		};
	}
}

#endif
