#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.h"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanHeader.hpp"

enum VkImageLayout;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class VulkanCommandAllocator;

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~VulkanCommandList();

			virtual void Begin() override final;
			virtual void End() override final;

			virtual void BeginRenderPass(RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override;

			inline vk::CommandBuffer& GetVulkanCommandBuffer() { return _CommandBuffer; }
			inline VulkanCommandAllocator& GetVulkanCommandAllocator() { return static_cast<VulkanCommandAllocator&>(GetCommandAllocator()); }

		private:
			Device&				_Device;
			vk::CommandBuffer	_CommandBuffer;
		};
	}
}
