#include "Vulkan/VulkanCommandList.hpp"

#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.h"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		VulkanCommandList::VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InDevice, InCommandAllocator)
		{
			vk::CommandBufferAllocateInfo CommandBufferAllocateInfo(
				static_cast<VulkanCommandAllocator&>(InCommandAllocator).GetVulkanCommandPool(),
				vk::CommandBufferLevel::ePrimary,
				1
			);
			Vulkan::VerifySuccess(
				GetVulkanDevice().GetVulkanDevice().allocateCommandBuffers(
					&CommandBufferAllocateInfo,
					&_CommandBuffer
				)
			);
		}

		VulkanCommandList::~VulkanCommandList()
		{
			GetVulkanDevice().GetVulkanDevice().freeCommandBuffers(
				GetVulkanCommandAllocator().GetVulkanCommandPool(),
				1, &_CommandBuffer
			);
		}

		void VulkanCommandList::Begin(_In_ GraphicsContext& InContext)
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
				memcpy(ClearColorValues[RenderTargetIndex].float32.data(), InRenderTargets[RenderTargetIndex].RenderTarget->GetResource().GetClearValue(), sizeof(float) * ClearColorValues[RenderTargetIndex].float32.size());
				
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

		void VulkanCommandList::Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			using namespace Eternal::Graphics::Vulkan;

			static constexpr vk::ImageSubresourceRange DefaultImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor,
				1, 0,
				1, 0
			);

			std::array<vk::BufferMemoryBarrier, MaxBufferTransitionsPerSubmit> BufferTransitions;
			std::array<vk::ImageMemoryBarrier, MaxTextureTransitionsPerSubmit> ImageTransitions;
			BufferTransitions.fill(vk::BufferMemoryBarrier());
			ImageTransitions.fill(vk::ImageMemoryBarrier());

			vk::PipelineStageFlagBits BeforeStages	= vk::PipelineStageFlagBits::eNoneKHR;
			vk::PipelineStageFlagBits AfterStages	= vk::PipelineStageFlagBits::eNoneKHR;

			QueueFamilyIndicesType QueueFamilyIndices;
			GetVulkanDevice().GetQueueFamilyIndices(QueueFamilyIndices);

			uint32_t BufferTransitionsCount = 0;
			uint32_t ImageTransitionsCount = 0;

			for (uint32_t ResourceTransitionIndex = 0; ResourceTransitionIndex < InResourceTransitionsCount; ++ResourceTransitionIndex)
			{
				ResourceTransition& CurrentResourceTransition		= InResourceTransitions[ResourceTransitionIndex];
				VulkanResource& VkResource							= static_cast<VulkanResource&>(CurrentResourceTransition.GetResource());
				const VulkanView* ResourceView						= CurrentResourceTransition.IsResource() ? nullptr : static_cast<VulkanView*>(CurrentResourceTransition.ViewToTransition);
				uint32_t BeforeQueueFamilyIndex						= QueueFamilyIndices[static_cast<uint32_t>(CurrentResourceTransition.BeforeCommandType)];
				uint32_t AfterQueueFamilyIndex						= QueueFamilyIndices[static_cast<uint32_t>(CurrentResourceTransition.AfterCommandType)];

				const TransitionState& BeforeState	= CurrentResourceTransition.GetBefore();
				const TransitionState& AfterState	= CurrentResourceTransition.GetAfter();

				BeforeStages	|= ConvertCommandTypeAndTransitionStateToPipelineStageFlags(CurrentResourceTransition.BeforeCommandType, BeforeState);
				AfterStages		|= ConvertCommandTypeAndTransitionStateToPipelineStageFlags(CurrentResourceTransition.BeforeCommandType, AfterState);

				switch (VkResource.GetResourceType())
				{
				case ResourceType::RESOURCE_TYPE_TEXTURE:
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
				case ResourceType::RESOURCE_TYPE_BUFFER:
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

		void VulkanCommandList::SetGraphicsPipeline(_In_ const Pipeline& InPipeline)
		{
			_CommandBuffer.bindPipeline(
				vk::PipelineBindPoint::eGraphics,
				static_cast<const VulkanPipeline&>(InPipeline).GetVulkanPipeline()
			);
		}

		void VulkanCommandList::SetGraphicsDescriptorTable(_In_ DescriptorTable& InDescriptorTable)
		{
			ETERNAL_BREAK();
		}

		void VulkanCommandList::DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_CommandBuffer.draw(
				InVertexCountPerInstance,
				InInstanceCount,
				InFirstVertex,
				InFirstInstance
			);
		}

		void VulkanCommandList::DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstIndex /* = 0 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_CommandBuffer.drawIndexed(
				InIndexCountPerInstance,
				InInstanceCount,
				InFirstIndex,
				InFirstVertex,
				InFirstInstance
			);
		}

		void VulkanCommandList::CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			ETERNAL_BREAK();
		}
	}
}
