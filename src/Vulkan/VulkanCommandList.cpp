#include "Vulkan/VulkanCommandList.hpp"

#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.h"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct VulkanPipelineFlags
		{
			VkAccessFlags			AccessFlags;
			VkPipelineStageFlags	PipelineStageFlags;
		};

		static const VulkanPipelineFlags PipelineFlags[] =
		{
			{ VK_ACCESS_INDIRECT_COMMAND_READ_BIT,							VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT },
			{ VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT,	VK_PIPELINE_STAGE_TRANSFER_BIT },
			{
				VK_ACCESS_INDEX_READ_BIT
				| VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
				| VK_ACCESS_UNIFORM_READ_BIT
				| VK_ACCESS_INPUT_ATTACHMENT_READ_BIT
				| VK_ACCESS_SHADER_READ_BIT
				| VK_ACCESS_SHADER_WRITE_BIT
				| VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
				| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
				| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
				| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
				| VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
				//| VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT
				//| VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT
				//| VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT
				| VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
				| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
				| VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				| VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
			},
			{
				0x0,
				VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
				| VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
				| VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
				| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
				| VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			}
		};

		static inline VkAccessFlags BuildAccessFlags(_In_ const TransitionState& State)
		{
			const VkAccessFlags AllAccessFlags =
				VK_ACCESS_INDIRECT_COMMAND_READ_BIT
				| VK_ACCESS_INDEX_READ_BIT
				| VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
				| VK_ACCESS_UNIFORM_READ_BIT
				| VK_ACCESS_INPUT_ATTACHMENT_READ_BIT
				| VK_ACCESS_SHADER_READ_BIT
				| VK_ACCESS_SHADER_WRITE_BIT
				| VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
				| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
				| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
				| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
				| VK_ACCESS_TRANSFER_READ_BIT
				| VK_ACCESS_TRANSFER_WRITE_BIT
				| VK_ACCESS_HOST_READ_BIT
				| VK_ACCESS_HOST_WRITE_BIT;

			return (VkAccessFlags)(VkAccessFlags(State) & AllAccessFlags);
		}

		static const VkImageAspectFlagBits BuildImageAspectFlags(_In_ const TransitionState& State)
		{
			if ((State & (TransitionState::TRANSITION_DEPTH_STENCIL_READ | TransitionState::TRANSITION_DEPTH_STENCIL_WRITE)) != TransitionState::TRANSITION_UNDEFINED)
				return VkImageAspectFlagBits(VK_IMAGE_ASPECT_DEPTH_BIT);// | VK_IMAGE_ASPECT_STENCIL_BIT);
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}

		VkImageLayout BuildImageLayout(_In_ const TransitionState& State)
		{
			if (State == TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_UNDEFINED;
			else if ((State & TransitionState::TRANSITION_RENDER_TARGET_WRITE) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			else if ((State & TransitionState::TRANSITION_DEPTH_STENCIL_WRITE) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			else if ((State & TransitionState::TRANSITION_DEPTH_STENCIL_READ) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			else if ((State & TransitionState::TRANSITION_COPY_READ) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			else if ((State & TransitionState::TRANSITION_COPY_WRITE) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			else if ((State & TransitionState::TRANSITION_PRESENT) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			else if ((State & TransitionState::TRANSITION_PREINITIALIZED) != TransitionState::TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_PREINITIALIZED;
			else
				return VK_IMAGE_LAYOUT_GENERAL;
			//VK_IMAGE_LAYOUT_PREINITIALIZED
		}

		static VkPipelineStageFlags BuildPipelineStageFlags(_In_ const TransitionState& State)
		{
			const TransitionState TransferState = (TransitionState::TRANSITION_COPY_READ
				| TransitionState::TRANSITION_COPY_WRITE
				| TransitionState::TRANSITION_CPU_READ
				| TransitionState::TRANSITION_CPU_WRITE);

			if ((State & TransferState) != TransitionState::TRANSITION_UNDEFINED)
				return VK_PIPELINE_STAGE_TRANSFER_BIT;
			else
				return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
					| VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
					| VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
					| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
					| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
					| VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}

		VulkanCommandList::VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InCommandAllocator)
			, _Device(InDevice)
		{
			vk::CommandBufferAllocateInfo CommandBufferAllocateInfo(
				static_cast<VulkanCommandAllocator&>(InCommandAllocator).GetVulkanCommandPool(),
				vk::CommandBufferLevel::ePrimary,
				1
			);
			Vulkan::VerifySuccess(
				static_cast<VulkanDevice&>(InDevice).GetVulkanDevice().allocateCommandBuffers(
					&CommandBufferAllocateInfo,
					&_CommandBuffer
				)
			);
		}

		VulkanCommandList::~VulkanCommandList()
		{
			static_cast<VulkanDevice&>(_Device).GetVulkanDevice().freeCommandBuffers(
				GetVulkanCommandAllocator().GetVulkanCommandPool(),
				1, &_CommandBuffer
			);
		}

		void VulkanCommandList::Begin()
		{
			vk::CommandBufferInheritanceInfo CommandBufferInheritanceInfo;
			vk::CommandBufferBeginInfo CommandBufferBeginInfo(
				vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
				&CommandBufferInheritanceInfo
			);

			_CommandBuffer.begin(CommandBufferBeginInfo);
		}

		void VulkanCommandList::End()
		{
			_CommandBuffer.end();
		}

		void VulkanCommandList::BeginRenderPass(RenderPass& InRenderPass)
		{
			vk::RenderPassBeginInfo RenderPassInfo(
				static_cast<VulkanRenderPass&>(InRenderPass).GetVulkanRenderPass(),
				static_cast<VulkanRenderPass&>(InRenderPass).GetFrameBuffer(),
				Vulkan::ConvertViewportToRect2D(InRenderPass.GetViewport()),
				0, nullptr
			);
			_CommandBuffer.beginRenderPass(
				&RenderPassInfo, vk::SubpassContents::eInline
			);
		}

		void VulkanCommandList::EndRenderPass()
		{
			_CommandBuffer.endRenderPass();
		}
	}
}
