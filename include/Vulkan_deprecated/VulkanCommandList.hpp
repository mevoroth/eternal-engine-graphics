#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.h"

struct VkCommandBuffer_T;
struct VkPipelineLayout_T;
struct VkDescriptorSet_T;
enum VkImageLayout;

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;
		class Device;
		//class VulkanCommandQueue;
		class VulkanPipeline;
		class RenderTarget;
		class VulkanRenderPass;
		class CommandAllocator;
		class Pipeline;
		class RootSignature;
		class Resource;
		class DescriptorTable;

		VkImageLayout BuildImageLayout(const TransitionState& State);

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type);
			~VulkanCommandList();
		};
	}
}
