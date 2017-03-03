#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

struct VkQueue_T;
struct VkCommandPool_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& Device);
			~VulkanCommandQueue();

			void Reset(_In_ uint32_t FrameIndex);
			VkCommandPool_T*& GetCommandPool() { return _CommandPool; }

		private:
			VulkanDevice&		_Device;
			VkQueue_T*			_CommandQueue	= nullptr;
			VkCommandPool_T*	_CommandPool	= nullptr;
		};
	}
}

#endif
