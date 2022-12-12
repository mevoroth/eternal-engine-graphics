#include "Vulkan/VulkanCommandList.hpp"

#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.h"
#include "Graphics/RootSignature.hpp"
#include "Graphics/DescriptorTable.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "Vulkan/VulkanDescriptorTable.hpp"
#include "Vulkan/VulkanViewport.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		VulkanCommandList::VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InDevice, InCommandAllocator)
		{
			vk::CommandBufferAllocateInfo CommandBufferAllocateInfo(
				static_cast<VulkanCommandAllocator&>(InCommandAllocator).GetVulkanCommandPool(),
				vk::CommandBufferLevel::ePrimary,
				1
			);
			VerifySuccess(
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

		void VulkanCommandList::SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName)
		{
			CommandList::SetName(InContext, InName);

			vk::Device InVulkanDevice = static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice();

			VkCommandBuffer CommandBufferHandle = _CommandBuffer;
			vk::DebugUtilsObjectNameInfoEXT ObjectNameInfo(
				vk::ObjectType::eCommandBuffer,
				reinterpret_cast<uint64_t>(CommandBufferHandle),
				InName.c_str()
			);

			VerifySuccess(InVulkanDevice.setDebugUtilsObjectNameEXT(&ObjectNameInfo, static_cast<VulkanDevice&>(InContext.GetDevice()).GetDispatchLoader()));
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
			ETERNAL_PROFILER(INFO)();
			CommandList::BeginRenderPass(InRenderPass);

			std::array<vk::ClearValue, MAX_RENDER_TARGETS_AND_DEPTH_STENCIL> ClearValues;
			ClearValues.fill(vk::ClearValue());
			std::array<vk::ClearColorValue, MAX_RENDER_TARGETS_AND_DEPTH_STENCIL> ClearColorValues;

			const vector<RenderTargetInformation>& InRenderTargets = InRenderPass.GetRenderTargets();
			for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < InRenderTargets.size(); ++RenderTargetIndex)
			{
				memcpy(ClearColorValues[RenderTargetIndex].float32.data(), InRenderTargets[RenderTargetIndex].RenderTarget->GetResource().GetClearValue(), sizeof(float) * ClearColorValues[RenderTargetIndex].float32.size());
				
				ClearValues[RenderTargetIndex] = vk::ClearValue(
					ClearColorValues[RenderTargetIndex]
				);
			}

			if (InRenderPass.GetDepthStencilRenderTarget())
			{
				uint32_t DepthStencilSlot = static_cast<uint32_t>(InRenderTargets.size());
				memcpy(ClearColorValues[DepthStencilSlot].float32.data(), InRenderPass.GetDepthStencilRenderTarget()->GetResource().GetClearValue(), sizeof(float) * ClearColorValues[DepthStencilSlot].float32.size());
				
				ClearValues[DepthStencilSlot] = vk::ClearValue(
					ClearColorValues[DepthStencilSlot]
				);
			}

			vk::RenderPassBeginInfo RenderPassInfo(
				static_cast<const VulkanRenderPass&>(InRenderPass).GetVulkanRenderPass(),
				static_cast<const VulkanRenderPass&>(InRenderPass).GetFrameBuffer(),
				ConvertViewportToRect2D(InRenderPass.GetViewport()),
				static_cast<uint32_t>(InRenderTargets.size()) + (InRenderPass.GetDepthStencilRenderTarget() ? 1 : 0), ClearValues.data()
			);
			_CommandBuffer.beginRenderPass(
				&RenderPassInfo, vk::SubpassContents::eInline
			);
		}

		void VulkanCommandList::EndRenderPass()
		{
			CommandList::EndRenderPass();

			_CommandBuffer.endRenderPass();
			_CurrentShaderStages = InvalidShaderStages;
		}

		void VulkanCommandList::Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			ETERNAL_PROFILER(INFO)();
			static constexpr vk::ImageSubresourceRange DefaultImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor,
				0, 1,
				0, 1
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
				AfterStages		|= ConvertCommandTypeAndTransitionStateToPipelineStageFlags(CurrentResourceTransition.AfterCommandType, AfterState);

				switch (VkResource.GetResourceType())
				{
				case ResourceType::RESOURCE_TYPE_TEXTURE:
				{
					ImageTransitions[ImageTransitionsCount++] = vk::ImageMemoryBarrier(
						ConvertTransitionStateToVulkanAccessFlags(BeforeState),
						ConvertTransitionStateToVulkanAccessFlags(AfterState),
						ConvertTransitionStateToVulkanImageLayout(BeforeState),
						ConvertTransitionStateToVulkanImageLayout(AfterState),
						BeforeQueueFamilyIndex,
						AfterQueueFamilyIndex,
						VkResource.GetVulkanImage(),
						ResourceView ? ResourceView->GetVulkanSubresourceRange() : DefaultImageSubresourceRange
					);
				} break;
				case ResourceType::RESOURCE_TYPE_BUFFER:
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

		void VulkanCommandList::SetViewport(_In_ const Viewport& InViewport)
		{
			const VulkanViewport& InVkViewport = static_cast<const VulkanViewport&>(InViewport);
			vk::Viewport ViewportInfo(
				static_cast<float>(InVkViewport.GetX()),
				static_cast<float>(InVkViewport.GetInvertedY()),
				static_cast<float>(InVkViewport.GetWidth()),
				static_cast<float>(InVkViewport.GetInvertedHeight()),
				0.0f, 1.0f
			);

			_CommandBuffer.setViewport(
				0, 1, &ViewportInfo
			);
		}

		void VulkanCommandList::SetScissorRectangle(_In_ const ScissorRectangle& InScissorRectangle)
		{
			vk::Rect2D VkScissorRectangle(
				vk::Offset2D(InScissorRectangle.Left, InScissorRectangle.Top),
				vk::Extent2D(
					InScissorRectangle.Right - InScissorRectangle.Left,
					InScissorRectangle.Bottom - InScissorRectangle.Top
				)
			);
			_CommandBuffer.setScissor(
				0, 1, &VkScissorRectangle
			);
		}

		void VulkanCommandList::SetGraphicsPipeline(_In_ const Pipeline& InPipeline)
		{
			_CommandBuffer.bindPipeline(
				vk::PipelineBindPoint::eGraphics,
				static_cast<const VulkanPipeline&>(InPipeline).GetVulkanPipeline()
			);

			const Viewport& InViewport = InPipeline.GetViewport();
			SetViewport(InViewport);

			ScissorRectangle Scissor =
			{
				InViewport.GetX(),
				InViewport.GetY(),
				InViewport.GetX() + InViewport.GetWidth(),
				InViewport.GetY() + InViewport.GetHeight()
			};
			SetScissorRectangle(Scissor);

			SetCurrentRootSignature(&InPipeline.GetRootSignature());
			_CurrentShaderStages = ConvertShaderTypeFlagsToVulkanShaderStageFlags(InPipeline.GetShaderTypes());
		}

		void VulkanCommandList::SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset /* = 0 */, _In_ const IndexBufferType& InIndexBufferType /* = IndexBufferType::INDEX_BUFFER_TYPE_16BITS */)
		{
			const VulkanResource& InVkIndexBuffer = static_cast<const VulkanResource&>(InIndexBuffer);

			_CommandBuffer.bindIndexBuffer(
				InVkIndexBuffer.GetVulkanBuffer(),
				InOffset,
				ConvertIndexBufferTypeToVulkanIndexType(InIndexBufferType)
			);
		}

		void VulkanCommandList::SetVertexBuffers(_In_ const Resource* InVertexBuffers[], _In_ uint32_t InBufferCount /* = 1 */, _In_ uint32_t InFirstVertexBuffer /* = 0 */, _In_ VertexBufferParameters InParameters[] /* = */)
		{
			ETERNAL_ASSERT((InFirstVertexBuffer + InBufferCount) <= MaxVertexBuffers);
			std::array<vk::Buffer, MaxVertexBuffers> InVkBuffers;
			std::array<vk::DeviceSize, MaxVertexBuffers> InOffsets;

			uint32_t CurrentBufferIndex = 0;
			for (uint32_t VertexBufferIndex = 0; VertexBufferIndex < InBufferCount; ++VertexBufferIndex)
			{
				InVkBuffers[CurrentBufferIndex]	= static_cast<const VulkanResource*>(InVertexBuffers[CurrentBufferIndex])->GetVulkanBuffer();
				InOffsets[CurrentBufferIndex]	= InParameters ? InParameters[CurrentBufferIndex].Offset : 0;
				++CurrentBufferIndex;
			}

			_CommandBuffer.bindVertexBuffers(
				InFirstVertexBuffer,
				InBufferCount,
				InVkBuffers.data(),
				InOffsets.data()
			);
		}
		
		void VulkanCommandList::SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			ETERNAL_PROFILER(INFO)();
			CommandList::SetGraphicsDescriptorTable(InContext, InDescriptorTable);
			_SetDescriptorTable<vk::PipelineBindPoint::eGraphics>(InContext, InDescriptorTable);
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

		void VulkanCommandList::SetComputePipeline(_In_ const Pipeline& InPipeline)
		{
			_CommandBuffer.bindPipeline(
				vk::PipelineBindPoint::eCompute,
				static_cast<const VulkanPipeline&>(InPipeline).GetVulkanPipeline()
			);
			SetCurrentRootSignature(&InPipeline.GetRootSignature());
			_CurrentShaderStages = ConvertShaderTypeFlagsToVulkanShaderStageFlags(InPipeline.GetShaderTypes());
		}

		void VulkanCommandList::SetComputeDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			ETERNAL_PROFILER(INFO)();
			CommandList::SetGraphicsDescriptorTable(InContext, InDescriptorTable);
			_SetDescriptorTable<vk::PipelineBindPoint::eCompute>(InContext, InDescriptorTable);
		}

		void VulkanCommandList::Dispatch(_In_ uint32_t InX /* = 1 */, _In_ uint32_t InY /* = 1 */, _In_ uint32_t InZ /* = 1 */)
		{
			CommandList::Dispatch(InX, InY, InZ);
			_CommandBuffer.dispatch(InX, InY, InZ);
		}

		void VulkanCommandList::DispatchMesh(_In_ uint32_t InTaskBatchesCount /* = 1 */)
		{
			ETERNAL_BREAK();
		}

		void VulkanCommandList::CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			ETERNAL_PROFILER(INFO)();
			if (InSourceResource.GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER)
			{
				if (InDestinationResource.GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER)
				{
					_CopyBufferToBuffer(InDestinationResource, InSourceResource, InCopyRegion);
				}
				else
				{
					ETERNAL_ASSERT(InDestinationResource.GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
					_CopyBufferToTexture(InDestinationResource, InSourceResource, InCopyRegion);
				}
			}
			else
			{
				ETERNAL_ASSERT(InSourceResource.GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
				if (InDestinationResource.GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER)
				{
					_CopyTextureToBuffer(InDestinationResource, InSourceResource, InCopyRegion);
				}
				else
				{
					ETERNAL_ASSERT(InDestinationResource.GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
					_CopyTextureToTexture(InDestinationResource, InSourceResource, InCopyRegion);
				}
			}
		}

		void VulkanCommandList::_CopyBufferToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			ETERNAL_BREAK();
		}

		void VulkanCommandList::_CopyTextureToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			vk::ImageCopy ImageCopy(
				vk::ImageSubresourceLayers(
					vk::ImageAspectFlagBits::eColor,
					InCopyRegion.Texture.SourceMIPIndex,
					InCopyRegion.Texture.Source.GetArraySlice(InSourceResource.GetResourceDimension()),
					InSourceResource.GetArraySize()
				),
				vk::Offset3D(
					InCopyRegion.Texture.Source.X,
					InCopyRegion.Texture.Source.Y,
					InCopyRegion.Texture.Source.Z
				),
				vk::ImageSubresourceLayers(
					vk::ImageAspectFlagBits::eColor,
					InCopyRegion.Texture.DestinationMIPIndex,
					InCopyRegion.Texture.Destination.GetArraySlice(InDestinationResource.GetResourceDimension()),
					InDestinationResource.GetArraySize()
				),
				vk::Offset3D(
					InCopyRegion.Texture.Destination.X,
					InCopyRegion.Texture.Destination.Y,
					InCopyRegion.Texture.Destination.Z
				),
				vk::Extent3D(
					InCopyRegion.Texture.Extent.Width,
					InCopyRegion.Texture.Extent.Height,
					InCopyRegion.Texture.Extent.Depth
				)
			);

			_CommandBuffer.copyImage(
				static_cast<const VulkanResource&>(InSourceResource).GetVulkanImage(),
				ConvertTransitionStateToVulkanImageLayout(InSourceResource.GetResourceState()),
				static_cast<const VulkanResource&>(InDestinationResource).GetVulkanImage(),
				ConvertTransitionStateToVulkanImageLayout(InDestinationResource.GetResourceState()),
				1, &ImageCopy
			);
		}

		void VulkanCommandList::_CopyBufferToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			vk::BufferImageCopy BufferToTextureCopy(
				InCopyRegion.TextureFromBuffer.SourceOffset,
				InCopyRegion.TextureFromBuffer.DestinationExtent.Width,
				//InCopyRegion.TextureFromBuffer.SourceSize / InCopyRegion.TextureFromBuffer.DestinationExtent.Height,
				InCopyRegion.TextureFromBuffer.DestinationExtent.Height,
				vk::ImageSubresourceLayers(
					vk::ImageAspectFlagBits::eColor,
					InCopyRegion.TextureFromBuffer.DestinationMIPIndex,
					InCopyRegion.TextureFromBuffer.Destination.GetArraySlice(InDestinationResource.GetResourceDimension()),
					InDestinationResource.GetArraySize()
				),
				vk::Offset3D(
					InCopyRegion.TextureFromBuffer.Destination.X,
					InCopyRegion.TextureFromBuffer.Destination.Y,
					InCopyRegion.TextureFromBuffer.Destination.Z
				),
				vk::Extent3D(
					InCopyRegion.TextureFromBuffer.DestinationExtent.Width,
					InCopyRegion.TextureFromBuffer.DestinationExtent.Height,
					InCopyRegion.TextureFromBuffer.DestinationExtent.Depth
				)
			);
			_CommandBuffer.copyBufferToImage(
				static_cast<const VulkanResource&>(InSourceResource).GetVulkanBuffer(),
				static_cast<const VulkanResource&>(InDestinationResource).GetVulkanImage(),
				ConvertTransitionStateToVulkanImageLayout(InDestinationResource.GetResourceState()),
				1, &BufferToTextureCopy
			);
		}

		void VulkanCommandList::_CopyTextureToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			ETERNAL_BREAK();
		}
		
		template<vk::PipelineBindPoint VulkanPipelineBindPoint>
		void VulkanCommandList::_SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			
			const VulkanRootSignature& VkRootSignature					= *static_cast<const VulkanRootSignature*>(GetCurrentSignature());
			const vk::PipelineLayout& CurrentPipelineLayout				= VkRootSignature.GetVulkanPipelineLayout();

			const RootSignatureCreateInformation& DescriptorTableLayout	= GetCurrentSignature()->GetCreateInformation();
			const vector<RootSignatureConstants>& Constants				= DescriptorTableLayout.Constants;
			const vector<RootSignatureParameter>& Parameters			= DescriptorTableLayout.Parameters;
			
			const vector<DescriptorTableConstants>& InConstants			= static_cast<const DescriptorTable&>(InDescriptorTable).GetConstants();
			const vector<DescriptorTableResource>& InResources			= static_cast<const DescriptorTable&>(InDescriptorTable).GetResources();
			ResourcesDirtyFlagsType& InConstantsDirtyFlags				= InDescriptorTable.GetConstantsDirtyFlags();
			ResourcesDirtyFlagsType& InResourcesDirtyFlags				= InDescriptorTable.GetResourcesDirtyFlags();

			//////////////////////////////////////////////////////////////////////////
			// Constants

			const vector<vk::PushConstantRange>& ConstantRanges			= VkRootSignature.GetVulkanPushConstantRanges();

			uint32_t ConstantRangeIndex = 0;
			for (uint32_t ConstantIndex = 0; ConstantIndex < Constants.size(); ++ConstantIndex)
			{
				if (InConstantsDirtyFlags.IsSet(ConstantIndex))
				{
					uint32_t ConstantOffset	= 0;
					uint32_t ConstantCount	= Constants[ConstantIndex].Count;
					while (ConstantOffset < ConstantCount)
					{
						const vk::PushConstantRange& ConstantRange = ConstantRanges[ConstantRangeIndex];

						uint32_t ConstantRangeConstantsCount = static_cast<int32_t>(ConstantRange.size - ConstantRange.offset) / ByteTo32Bits;
					
						_CommandBuffer.pushConstants(
							CurrentPipelineLayout,
							ConstantRange.stageFlags,
							ConstantRange.offset,
							ConstantRange.size,
							&InConstants[ConstantIndex].Constants[ConstantOffset]
						);

						ConstantOffset += ConstantRangeConstantsCount;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// Descriptor sets

			std::array<vk::DescriptorSet, MaxDescriptorSetsCount> DescriptorSets;
			DescriptorSets.fill(vk::DescriptorSet());

			VulkanDescriptorTable& VkDescriptorTable = static_cast<VulkanDescriptorTable&>(InDescriptorTable);

			uint32_t DescriptorSetCount = 0;
			VkDescriptorTable.Commit(InContext, Parameters);
			for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex)
			{
				if (Parameters[ParameterIndex].Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				{
					const vector<DescriptorTableResource>& Resources = static_cast<const DescriptorTable&>(InDescriptorTable).GetResources();
					const VulkanDescriptorTable* VkTable = static_cast<const VulkanDescriptorTable*>(Resources[ParameterIndex].ResourceDescriptorTable);
					const_cast<VulkanDescriptorTable*>(VkTable)->Commit(
						InContext,
						Parameters[ParameterIndex].DescriptorTable
					);
					DescriptorSets[++DescriptorSetCount] = VkTable->GetVulkanDescriptorSet(InContext.GetCurrentFrameIndex());
				}
			}

			const vk::DescriptorSet* FirstSetPointer = &DescriptorSets[1];
			if (Parameters.size() > DescriptorSetCount)
			{
				DescriptorSets[0] = VkDescriptorTable.GetVulkanDescriptorSet(InContext.GetCurrentFrameIndex());
				FirstSetPointer = &DescriptorSets[0];
				++DescriptorSetCount;
			}
			
			_CommandBuffer.bindDescriptorSets(
				VulkanPipelineBindPoint,
				CurrentPipelineLayout,
				0,
				DescriptorSetCount,
				FirstSetPointer,
				0, nullptr
			);
		}
	}
}
