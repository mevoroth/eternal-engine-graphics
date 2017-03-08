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
<<<<<<< HEAD
<<<<<<< Updated upstream
=======
		class VulkanCommandList;
		class VulkanFence;
>>>>>>> Stashed changes
=======
		class VulkanCommandList;
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& Device);
			~VulkanCommandQueue();

			void Reset(_In_ uint32_t FrameIndex);
<<<<<<< HEAD
<<<<<<< Updated upstream
=======
			void Flush(_In_ VulkanFence& FenceObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount);
>>>>>>> Stashed changes
=======
			void Flush(_In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount);
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
			VkCommandPool_T*& GetCommandPool() { return _CommandPool; }

		private:
			VulkanDevice&		_Device;
			VkQueue_T*			_CommandQueue	= nullptr;
			VkCommandPool_T*	_CommandPool	= nullptr;
		};
	}
}

#endif
