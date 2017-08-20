#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#include <vector>
#include <cstdint>

#include "Graphics/SwapChain.hpp"

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

			virtual void AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext) override;
			virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext) override;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) override;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) override;
			virtual uint32_t GetBackBuffersFrameCount() const override;
			virtual RenderPass& GetMainRenderPass() override;
			VkSwapchainKHR_T*& GetSwapChain() { return _SwapChain; }

			VkSemaphore_T*& GetAcquireSemaphore(_In_ uint32_t ResourceIndex);

		private:
			vector<VulkanRenderPass*>	_RenderPasses;
			vector<VulkanResource*>		_BackBuffers;
			vector<View*>				_BackBufferViews;
			vector<VkSemaphore_T*>		_AcquireSemaphores;
			VkSurfaceKHR_T*				_Surface	= nullptr;
			VkSwapchainKHR_T*			_SwapChain	= nullptr;
			uint32_t					_FrameIndex	= 0u;
		};
	}
}

#endif
