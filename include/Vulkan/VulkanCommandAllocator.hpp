#pragma once

#include "Graphics/CommandAllocator.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		class VulkanCommandAllocator final : public CommandAllocator
		{
		public:
			VulkanCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue);
			virtual ~VulkanCommandAllocator();

			virtual void Reset() override final;

			inline vk::CommandPool& GetVulkanCommandPool() { return _CommandPool; }

		private:
			Device&				_Device;
			vk::CommandPool		_CommandPool;
		};
	}
}
