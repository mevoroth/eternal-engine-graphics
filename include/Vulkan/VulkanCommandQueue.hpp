#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include <vector>
#include <cstdint>

struct VkQueue_T;
struct VkCommandPool_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class VulkanDevice;
		class VulkanCommandList;
		class VulkanFence;
		class VulkanSwapChain;

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj);
			~VulkanCommandQueue();

			void Reset(_In_ uint32_t FrameIndex);
			VkCommandPool_T*& GetCommandPool() { return _CommandPool; }
			VkSemaphore_T*& GetCompletedSemaphore(_In_ uint32_t ResourceIndex);
			VkQueue_T* GetCommandQueue();

		private:
			vector<VkSemaphore_T*> _CompletedSemaphores;
			VulkanDevice&		_Device;
			VkQueue_T*			_CommandQueue	= nullptr;
			VkCommandPool_T*	_CommandPool	= nullptr;
		};
	}
}

#endif
