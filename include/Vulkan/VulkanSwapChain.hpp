<<<<<<< HEAD
#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#include <vector>

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;

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

		class VulkanSwapChain
		{
		public:
			VulkanSwapChain(_In_ VulkanDevice& DeviceObj, _In_ Window& WindowObj);

			VulkanFrameBuffer& GetBackBuffer(_In_ uint32_t BackBufferIndex);
			VulkanRenderPass* GetMainRenderPass() { return _RenderPass; }

		private:
			vector<VulkanFrameBuffer*> _BackBuffers;
			vector<VulkanView*> _BackBufferViews;
			VulkanRenderPass* _RenderPass	= nullptr;
			VkSurfaceKHR_T* _Surface		= nullptr;
			VkSwapchainKHR_T* _SwapChain	= nullptr;
		};
	}
}

#endif
=======
#ifndef _VULKAN_SWAP_CHAIN_HPP_
#define _VULKAN_SWAP_CHAIN_HPP_

#include <vector>

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window;
		class VulkanDevice;
		class VulkanFrameBuffer;
		class VulkanView;

		class VulkanSwapChain
		{
		public:
			VulkanSwapChain(_In_ VulkanDevice& DeviceObj, _In_ Window& WindowObj);

			VulkanFrameBuffer& GetBackBuffer(_In_ uint32_t BackBufferIndex);

		private:
			vector<VulkanFrameBuffer*> _BackBuffers;
			vector<VulkanView*> _BackBufferViews;
			VkSurfaceKHR_T* _Surface		= nullptr;
			VkSwapchainKHR_T* _SwapChain	= nullptr;
		};
	}
}

#endif
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
