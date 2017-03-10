#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include "Graphics/CommandQueue.hpp"
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

		class VulkanCommandQueue : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj);
			~VulkanCommandQueue();

			virtual void Reset(_In_ uint32_t FrameIndex) override;
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
