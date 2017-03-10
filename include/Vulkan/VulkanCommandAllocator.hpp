#ifndef _VULKAN_COMMAND_ALLOCATOR_HPP_
#define _VULKAN_COMMAND_ALLOCATOR_HPP_

#include "Graphics/CommandAllocator.hpp"

struct VkCommandPool_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanCommandAllocator : public CommandAllocator
		{
		public:
			VulkanCommandAllocator(_In_ Device& DeviceObj);
			virtual ~VulkanCommandAllocator();

			inline VkCommandPool_T*& GetVulkanCommandPool() { return _CommandPool; }
			inline VkSemaphore_T*& GetSemaphore() { return _CompletedSemaphore; }

		private:
			Device&				_Device;
			VkCommandPool_T*	_CommandPool		= nullptr;
			VkSemaphore_T*		_CompletedSemaphore = nullptr;
		};
	}
}

#endif
