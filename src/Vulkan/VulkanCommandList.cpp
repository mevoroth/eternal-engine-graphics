#include "Vulkan/VulkanCommandList.hpp"

#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.h"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
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

		void VulkanCommandList::BeginRenderPass(const RenderPass& InRenderPass)
		{
			std::array<vk::ClearValue, MAX_RENDER_TARGETS> ClearValues;
			ClearValues.fill(vk::ClearValue());
			std::array<vk::ClearColorValue, MAX_RENDER_TARGETS> ClearColorValues;

			const vector<RenderTargetInformation>& InRenderTargets = InRenderPass.GetRenderTargets();
			for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < InRenderTargets.size(); ++RenderTargetIndex)
			{
				memcpy(ClearColorValues[RenderTargetIndex].float32.data(), InRenderTargets[RenderTargetIndex].ClearValue, sizeof(float) * ClearColorValues[RenderTargetIndex].float32.size());
				
				ClearValues[RenderTargetIndex] = vk::ClearValue(
					ClearColorValues[RenderTargetIndex]
				);
			}

			vk::RenderPassBeginInfo RenderPassInfo(
				static_cast<const VulkanRenderPass&>(InRenderPass).GetVulkanRenderPass(),
				static_cast<const VulkanRenderPass&>(InRenderPass).GetFrameBuffer(),
				Vulkan::ConvertViewportToRect2D(InRenderPass.GetViewport()),
				static_cast<uint32_t>(InRenderTargets.size()), ClearValues.data()
			);
			_CommandBuffer.beginRenderPass(
				&RenderPassInfo, vk::SubpassContents::eInline
			);
		}

		void VulkanCommandList::EndRenderPass()
		{
			_CommandBuffer.endRenderPass();
		}

		void VulkanCommandList::Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			using namespace Eternal::Graphics::Vulkan;

			std::array<vk::BufferMemoryBarrier, MaxBufferTransitionsPerSubmit> BufferTransitions;
			std::array<vk::ImageMemoryBarrier, MaxTextureTransitionsPerSubmit> ImageTransitions;
			BufferTransitions.fill(vk::BufferMemoryBarrier());
			ImageTransitions.fill(vk::ImageMemoryBarrier());

			vk::PipelineStageFlagBits BeforeStages	= vk::PipelineStageFlagBits::eNoneKHR;
			vk::PipelineStageFlagBits AfterStages	= vk::PipelineStageFlagBits::eNoneKHR;

			QueueFamilyIndicesType QueueFamilyIndices;
			static_cast<VulkanDevice&>(_Device).GetQueueFamilyIndices(QueueFamilyIndices);

			uint32_t BufferTransitionsCount = 0;
			uint32_t ImageTransitionsCount = 0;

			for (uint32_t ResourceTransitionIndex = 0; ResourceTransitionIndex < InResourceTransitionsCount; ++ResourceTransitionIndex)
			{
				const ResourceTransition& CurrentResourceTransition	= InResourceTransitions[ResourceTransitionIndex];
				VulkanResource& VkResource							= static_cast<VulkanResource&>(CurrentResourceTransition.ResourceToTransition->GetResource());
				const VulkanView* ResourceView						= static_cast<VulkanView*>(CurrentResourceTransition.ResourceToTransition);
				uint32_t BeforeQueueFamilyIndex						= QueueFamilyIndices[static_cast<uint32_t>(CurrentResourceTransition.BeforeCommandType)];
				uint32_t AfterQueueFamilyIndex						= QueueFamilyIndices[static_cast<uint32_t>(CurrentResourceTransition.AfterCommandType)];

				const TransitionState& BeforeState	= CurrentResourceTransition.GetBefore();
				const TransitionState& AfterState	= CurrentResourceTransition.GetAfter();

				BeforeStages	|= ConvertCommandTypeAndTransitionStateToPipelineStageFlags(CurrentResourceTransition.BeforeCommandType, BeforeState);
				AfterStages		|= ConvertCommandTypeAndTransitionStateToPipelineStageFlags(CurrentResourceTransition.BeforeCommandType, AfterState);

				switch (VkResource.GetVulkanResourceType())
				{
				case VulkanResourceType::BUFFER:
				{
					BufferTransitions[BufferTransitionsCount++] = vk::BufferMemoryBarrier(
						ConvertTransitionStateToVulkanAccessFlags(BeforeState),
						ConvertTransitionStateToVulkanAccessFlags(AfterState),
						BeforeQueueFamilyIndex,
						AfterQueueFamilyIndex,
						VkResource.GetVulkanBuffer(),
						0, VK_WHOLE_SIZE
					);
				} break;
				case VulkanResourceType::IMAGE:
				{
					ImageTransitions[ImageTransitionsCount++] = vk::ImageMemoryBarrier(
						ConvertTransitionStateToVulkanAccessFlags(BeforeState),
						ConvertTransitionStateToVulkanAccessFlags(AfterState),
						ConvertTransitionStateToVulkanImageLayout(BeforeState),
						ConvertTransitionStateToVulkanImageLayout(AfterState),
						BeforeQueueFamilyIndex,
						AfterQueueFamilyIndex,
						VkResource.GetVulkanImage(),
						ResourceView->GetVulkanSubresourceRange()
					);
				} break;
				}

				VkResource.SetResourceState(AfterState);
			}

			ETERNAL_ASSERT((BufferTransitionsCount + ImageTransitionsCount) == InResourceTransitionsCount);

			_CommandBuffer.pipelineBarrier(
				BeforeStages,
				AfterStages,
				vk::DependencyFlagBits(),
				0,  nullptr,
				BufferTransitionsCount, BufferTransitionsCount > 0 ? BufferTransitions.data() : nullptr,
				ImageTransitionsCount, ImageTransitionsCount > 0 ? ImageTransitions.data() : nullptr
			);
		}
	}
}
