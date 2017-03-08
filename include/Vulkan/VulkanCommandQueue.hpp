#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

struct VkQueue_T;
struct VkCommandPool_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
<<<<<<< Updated upstream
=======
		class VulkanCommandList;
		class VulkanFence;
>>>>>>> Stashed changes

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& Device);
			~VulkanCommandQueue();

			void Reset(_In_ uint32_t FrameIndex);
<<<<<<< Updated upstream
=======
			void Flush(_In_ VulkanFence& FenceObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount);
>>>>>>> Stashed changes
			VkCommandPool_T*& GetCommandPool() { return _CommandPool; }

		private:
			VulkanDevice&		_Device;
			VkQueue_T*			_CommandQueue	= nullptr;
			VkCommandPool_T*	_CommandPool	= nullptr;
		};
	}
}

#endif
