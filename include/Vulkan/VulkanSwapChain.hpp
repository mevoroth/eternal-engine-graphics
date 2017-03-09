#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#include <vector>
#include <cstdint>

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window;
		class VulkanDevice;
		class VulkanFrameBuffer;
		class VulkanView;
		class VulkanRenderPass;
		class VulkanCommandQueue;
		class VulkanFence;

		class VulkanSwapChain
		{
		public:
			VulkanSwapChain(_In_ VulkanDevice& DeviceObj, _In_ Window& WindowObj);

			uint32_t AcquireFrame(_In_ VulkanDevice& DeviceObj, _In_ VulkanFence& FenceObj);
			void Present(_In_ VulkanDevice& DeviceObj, _In_ VulkanCommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex);

			VulkanFrameBuffer& GetBackBuffer(_In_ uint32_t BackBufferIndex);
			uint32_t GetBackBuffersFrameCount() const;
			VkSwapchainKHR_T*& GetSwapChain() { return _SwapChain; }
			VulkanRenderPass* GetMainRenderPass() { return _RenderPass; }

			VkSemaphore_T*& GetAcquireSemaphore(_In_ uint32_t ResourceIndex);

		private:
			vector<VulkanFrameBuffer*> _BackBuffers;
			vector<VulkanView*> _BackBufferViews;
			vector<VkSemaphore_T*> _AcquireSemaphores;
			vector<VkSemaphore_T*> _PresentSemaphores;
			VulkanRenderPass* _RenderPass	= nullptr;
			VkSurfaceKHR_T* _Surface		= nullptr;
			VkSwapchainKHR_T* _SwapChain	= nullptr;
			uint32_t _FrameIndex			= 0u;
		};
	}
}

#endif
