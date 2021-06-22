#pragma once

#include "Graphics/CommandAllocator.hpp"
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		class VulkanCommandAllocator : public CommandAllocator
		{
		public:
			VulkanCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue);
			virtual ~VulkanCommandAllocator();

			inline vk::CommandPool& GetVulkanCommandPool() { return _CommandPool; }

		private:
			Device&				_Device;
			vk::CommandPool		_CommandPool;
		};
	}
}
