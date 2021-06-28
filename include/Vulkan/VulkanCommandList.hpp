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

			virtual void Begin(_In_ GraphicsContext& InContext) override final;
			virtual void End() override final;

			virtual void BeginRenderPass(const RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override final;

			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;

			inline vk::CommandBuffer& GetVulkanCommandBuffer() { return _CommandBuffer; }
			inline VulkanCommandAllocator& GetVulkanCommandAllocator() { return static_cast<VulkanCommandAllocator&>(GetCommandAllocator()); }

		private:
			Device&				_Device;
			vk::CommandBuffer	_CommandBuffer;
		};
	}
}
