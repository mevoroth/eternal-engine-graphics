#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.h"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include <vulkan/vulkan.hpp>

enum VkImageLayout;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class VulkanCommandAllocator;

		VkImageLayout BuildImageLayout(const TransitionState& State);

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~VulkanCommandList();

			virtual void Begin() override;
			virtual void End() override;

			inline vk::CommandBuffer& GetVulkanCommandBuffer() { return _CommandBuffer; }
			inline VulkanCommandAllocator& GetVulkanCommandAllocator() { return static_cast<VulkanCommandAllocator&>(GetCommandAllocator()); }

		private:
			Device&				_Device;
			vk::CommandBuffer	_CommandBuffer;
		};
	}
}
