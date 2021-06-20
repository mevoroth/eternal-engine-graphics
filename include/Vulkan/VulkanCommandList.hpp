#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.h"

enum VkImageLayout;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		VkImageLayout BuildImageLayout(const TransitionState& State);

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type);
			~VulkanCommandList();

			inline vk::CommandBuffer GetVulkanCommandBuffer() { return _CommandBuffer; }

		private:
			vk::CommandBuffer _CommandBuffer;
		};
	}
}
