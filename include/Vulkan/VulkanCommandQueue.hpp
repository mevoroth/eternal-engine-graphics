#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include <vulkan/vulkan.h>

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanCommandQueue
		{
		public:
			VulkanCommandQueue(_In_ VulkanDevice& Device);

		private:
			VkQueue _CommandQueue = nullptr;
		};
	}
}

#endif
