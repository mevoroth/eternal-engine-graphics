#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include <cstdint>

struct VkQueue_T;
struct VkCommandPool_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
		class VulkanCommandList;
		class VulkanFence;

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& Device);
			~VulkanCommandQueue();

			void Reset(_In_ uint32_t FrameIndex);
			void Flush(_In_ VulkanFence& FenceObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount);
			VkCommandPool_T*& GetCommandPool() { return _CommandPool; }

		private:
			VulkanDevice&		_Device;
			VkQueue_T*			_CommandQueue	= nullptr;
			VkCommandPool_T*	_CommandPool	= nullptr;
		};
	}
}

#endif
