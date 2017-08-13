#include "Vulkan/VulkanCommandList.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "GraphicsSettings.hpp"
#include "Graphics/Viewport.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include "Vulkan/VulkanRenderTarget.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanHeap.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanDescriptorHeap.hpp"

using namespace Eternal::Graphics;

static inline VkAccessFlags BuildAccessFlags(const TransitionState& State)
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
		| VK_ACCESS_HOST_WRITE_BIT
		| VK_ACCESS_MEMORY_READ_BIT
		| VK_ACCESS_MEMORY_WRITE_BIT;

	if (State == TRANSITION_UNDEFINED)
		return	AllAccessFlags;

	return (VkAccessFlags)(State & AllAccessFlags);
}

namespace Eternal
{
	namespace Graphics
	{
		VkImageLayout BuildImageLayout(const TransitionState& State)
		{
			if (State == TRANSITION_UNDEFINED)
				return VK_IMAGE_LAYOUT_UNDEFINED;
			else if (State & TRANSITION_RENDER_TARGET_WRITE)
				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			else if (State & TRANSITION_DEPTH_STENCIL_WRITE)
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			else if (State & TRANSITION_DEPTH_STENCIL_READ)
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			else if (State & TRANSITION_COPY_READ)
				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			else if (State & TRANSITION_COPY_WRITE)
				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			else if (State & TRANSITION_PRESENT)
				return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			else
				return VK_IMAGE_LAYOUT_GENERAL;
			//VK_IMAGE_LAYOUT_PREINITIALIZED
		}
	}
}

VulkanCommandList::VulkanCommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
	: CommandList(DeviceObj, Type)
	, _Device(DeviceObj)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo;
	CommandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.pNext					= nullptr;
	CommandBufferAllocateInfo.commandPool			= static_cast<VulkanCommandAllocator&>(*GetCommandAllocator()).GetVulkanCommandPool();
	CommandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount	= 1;

	VkResult Result = vkAllocateCommandBuffers(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &CommandBufferAllocateInfo, &_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandList::~VulkanCommandList()
{
	vkFreeCommandBuffers(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), static_cast<VulkanCommandAllocator&>(*GetCommandAllocator()).GetVulkanCommandPool(), 1, &_CommandBuffer);
	_CommandBuffer = nullptr;
}

void VulkanCommandList::Begin(_In_ Pipeline& PipelineObj)
{
	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	CommandBufferBeginInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.pNext			= nullptr;
	CommandBufferBeginInfo.flags			= 0;
	CommandBufferBeginInfo.pInheritanceInfo = nullptr;

	VkResult Result = vkBeginCommandBuffer(_CommandBuffer, &CommandBufferBeginInfo);
	ETERNAL_ASSERT(!Result);

	vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<VulkanPipeline&>(PipelineObj).GetVulkanPipeline());
}

void VulkanCommandList::Begin()
{
	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	CommandBufferBeginInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.pNext			= nullptr;
	CommandBufferBeginInfo.flags			= 0;
	CommandBufferBeginInfo.pInheritanceInfo = nullptr;

	VkResult Result = vkBeginCommandBuffer(_CommandBuffer, &CommandBufferBeginInfo);
	ETERNAL_ASSERT(!Result);
}

void VulkanCommandList::BeginRenderPass(_In_ RenderPass& RenderPassObj)
{
	VkClearValue ClearValue;
	ClearValue.color.float32[0] = 0.0f;
	ClearValue.color.float32[1] = 0.0f;
	ClearValue.color.float32[2] = 0.0f;
	ClearValue.color.float32[3] = 0.0f;

	VkRenderPassBeginInfo RenderPassBeginInfo;
	RenderPassBeginInfo.sType						= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.pNext						= nullptr;
	RenderPassBeginInfo.renderPass					= static_cast<VulkanRenderPass&>(RenderPassObj).GetRenderPass();
	RenderPassBeginInfo.framebuffer					= static_cast<VulkanRenderPass&>(RenderPassObj).GetFrameBuffer();
	RenderPassBeginInfo.renderArea.offset.x			= RenderPassObj.GetViewport().GetX();
	RenderPassBeginInfo.renderArea.offset.y			= RenderPassObj.GetViewport().GetY();
	RenderPassBeginInfo.renderArea.extent.width		= RenderPassObj.GetViewport().GetWidth();
	RenderPassBeginInfo.renderArea.extent.height	= RenderPassObj.GetViewport().GetHeight();
	RenderPassBeginInfo.clearValueCount				= 1;
	RenderPassBeginInfo.pClearValues				= &ClearValue;

	vkCmdBeginRenderPass(_CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandList::EndRenderPass()
{
	vkCmdEndRenderPass(_CommandBuffer);
}

void VulkanCommandList::End()
{
	VkResult Result = vkEndCommandBuffer(_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

void VulkanCommandList::BindConstantBuffer(_In_ uint32_t Slot, _In_ View& ConstantBuffer)
{

}

void VulkanCommandList::BindDescriptorTable(_In_ uint32_t Slot, _In_ View& DescriptorTable)
{
	ETERNAL_ASSERT(Slot < _CommandListCache.DescriptorTablesCount);
	_CommandListCache.DescriptorTables[Slot]	= static_cast<VulkanView&>(DescriptorTable).GetDescriptorSet();
	_CommandListCache.Dirty						= true;
}

void VulkanCommandList::BindBuffer(_In_ uint32_t Slot, _In_ View& Buffer)
{

}

void VulkanCommandList::BindUAV(_In_ uint32_t Slot, _In_ View& UAV)
{

}

void VulkanCommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
	vkCmdDraw(_CommandBuffer, PrimitiveCount, 1, 0, 0);
}

void VulkanCommandList::DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation)
{
	vkCmdDrawIndexed(_CommandBuffer, IndicesCount, 1, StartIndexLocation, BaseVertexLocation, 0);
}

void VulkanCommandList::DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation)
{
	vkCmdDrawIndexed(_CommandBuffer, IndicesCount, InstancesCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

void VulkanCommandList::SetIndicesBuffer(_In_ Resource* IndicesBuffer)
{
	vkCmdBindIndexBuffer(_CommandBuffer, static_cast<VulkanResource*>(IndicesBuffer)->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
}

void VulkanCommandList::SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t VerticesBuffersCount, _In_ Resource* VerticesBuffers[])
{
	ETERNAL_ASSERT(VerticesBuffersCount <= MAX_VERTICES_BUFFERS);
	VkBuffer VerticesBuffersHandles[MAX_VERTICES_BUFFERS];
	VkDeviceSize VerticesOffsets[MAX_VERTICES_BUFFERS];
	for (uint32_t VerticesBufferIndex = 0; VerticesBufferIndex < VerticesBuffersCount; ++VerticesBufferIndex)
	{
		VerticesBuffersHandles[VerticesBufferIndex] = static_cast<VulkanResource*>(VerticesBuffers[VerticesBufferIndex])->GetBuffer();
		VerticesOffsets[VerticesBufferIndex] = 0ull;
	}
	vkCmdBindVertexBuffers(_CommandBuffer, StartSlot, VerticesBuffersCount, VerticesBuffersHandles, VerticesOffsets);
}

void VulkanCommandList::SetViewport(_In_ const Viewport& ViewportObj)
{
	VkViewport VulkanViewport;
	VulkanViewport.x		= ViewportObj.GetX();
	VulkanViewport.y		= ViewportObj.GetY();
	VulkanViewport.width	= ViewportObj.GetWidth();
	VulkanViewport.height	= ViewportObj.GetHeight();
	VulkanViewport.minDepth	= 0.0f;
	VulkanViewport.maxDepth	= 1.0f;

	vkCmdSetViewport(_CommandBuffer, 0, 1, &VulkanViewport);
}

void VulkanCommandList::SetScissorRectangle(_In_ const Viewport& ViewportObj)
{
	VkRect2D VulkanScissor;
	VulkanScissor.offset.x		= ViewportObj.GetX();
	VulkanScissor.offset.y		= ViewportObj.GetY();
	VulkanScissor.extent.width	= ViewportObj.GetWidth();
	VulkanScissor.extent.height = ViewportObj.GetHeight();

	vkCmdSetScissor(_CommandBuffer, 0, 1, &VulkanScissor);
}

void VulkanCommandList::BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount)
{
	//ETERNAL_ASSERT(DescriptorHeapsCount <= MAX_DESCRIPTORS_HEAPS);
	//VkDescriptorSet VulkanDescriptorSets[MAX_DESCRIPTORS_HEAPS];
	//for (uint32_t DescriptorSetIndex = 0; DescriptorSetIndex < DescriptorHeapsCount; ++DescriptorSetIndex)
	//{
	//	VulkanDescriptorSets[DescriptorSetIndex] = static_cast<VulkanDescriptorHeap*>(DescriptorHeaps[DescriptorSetIndex])->Bind();
	//}

	//vkCmdBindDescriptorSets(
	//	_CommandBuffer,
	//	VK_PIPELINE_BIND_POINT_GRAPHICS,
	//	static_cast<VulkanRootSignature&>(RootSignatureObj).GetPipelineLayout(),
	//	0,
	//	DescriptorHeapsCount,
	//	VulkanDescriptorSets,
	//	0,
	//	nullptr
	//);

	VulkanRootSignature& VkRootSignature = static_cast<VulkanRootSignature&>(RootSignatureObj);
	ETERNAL_ASSERT(VkRootSignature.GetParametersCount() < MAX_BINDABLE_RESOURCES);

	_CommandListCache.PipelineLayout		= VkRootSignature.GetPipelineLayout();
	_CommandListCache.DescriptorTablesCount	= VkRootSignature.GetParametersCount();
#ifdef ETERNAL_DEBUG
	memset(_CommandListCache.DescriptorTables, 0x0, VkRootSignature.GetParametersCount() * sizeof(VkDescriptorSet));
#endif
}

void VulkanCommandList::CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination)
{
	VkBufferCopy BufferCopy;
	BufferCopy.srcOffset	= 0ull;
	BufferCopy.dstOffset	= 0ull;
	BufferCopy.size			= Source.GetBufferSize();

	vkCmdCopyBuffer(_CommandBuffer, static_cast<VulkanResource&>(Source).GetBuffer(), static_cast<VulkanResource&>(Destination).GetBuffer(), 1, &BufferCopy);
}

void VulkanCommandList::CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination, uint64_t SourceOffset, uint64_t DestinationOffset, uint64_t Size)
{
	VkBufferCopy BufferCopy;
	BufferCopy.srcOffset	= SourceOffset;
	BufferCopy.dstOffset	= DestinationOffset;
	BufferCopy.size			= Size;

	vkCmdCopyBuffer(_CommandBuffer, static_cast<VulkanResource&>(Source).GetBuffer(), static_cast<VulkanResource&>(Destination).GetBuffer(), 1, &BufferCopy);
}

void VulkanCommandList::CopyTexture(_In_ Resource& Source, _In_ Resource& Destination, _In_ const Position3D& SourcePosition, _In_ const Position3D& DestinationPosition, _In_ const Extent& Size)
{
	ETERNAL_ASSERT(false);
}

void VulkanCommandList::CopyBufferToTexture(_In_ Resource& Buffer, _In_ Resource& Texture, uint64_t BufferOffset, uint64_t BufferSize, _In_ const Position3D& TexturePosition, _In_ const Extent& Size)
{
	ETERNAL_ASSERT(false);
}

void VulkanCommandList::Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount)
{
	vector<VkBufferMemoryBarrier> BufferMemoryBarriers;
	BufferMemoryBarriers.resize(BuffersCount);

	vector<VkImageMemoryBarrier> ImageMemoryBarriers;
	ImageMemoryBarriers.resize(ImagesCount);

	for (uint32_t BufferIndex = 0; BufferIndex < BuffersCount; ++BufferIndex)
	{
		VulkanResource* CurrentVulkanBuffer = static_cast<VulkanResource*>(Buffers[BufferIndex].ResourceObj);

		BufferMemoryBarriers[BufferIndex].sType					= VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		BufferMemoryBarriers[BufferIndex].pNext					= nullptr;
		BufferMemoryBarriers[BufferIndex].srcAccessMask			= BuildAccessFlags(Buffers[BufferIndex].Before);
		BufferMemoryBarriers[BufferIndex].dstAccessMask			= BuildAccessFlags(Buffers[BufferIndex].After);
		BufferMemoryBarriers[BufferIndex].srcQueueFamilyIndex	= 0;
		BufferMemoryBarriers[BufferIndex].dstQueueFamilyIndex	= 0;
		BufferMemoryBarriers[BufferIndex].buffer				= CurrentVulkanBuffer->GetBuffer();
		BufferMemoryBarriers[BufferIndex].offset				= 0ull;
		BufferMemoryBarriers[BufferIndex].size					= CurrentVulkanBuffer->GetBufferSize();
	}

	for (uint32_t ImageIndex = 0; ImageIndex < ImagesCount; ++ImageIndex)
	{
		VulkanResource* CurrentVulkanBuffer = static_cast<VulkanResource*>(Images[ImageIndex].ResourceObj);

		ImageMemoryBarriers[ImageIndex].sType							= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		ImageMemoryBarriers[ImageIndex].pNext							= nullptr;
		ImageMemoryBarriers[ImageIndex].srcAccessMask					= BuildAccessFlags(Images[ImageIndex].Before);
		ImageMemoryBarriers[ImageIndex].dstAccessMask					= BuildAccessFlags(Images[ImageIndex].After);
		ImageMemoryBarriers[ImageIndex].oldLayout						= BuildImageLayout(Images[ImageIndex].Before);
		ImageMemoryBarriers[ImageIndex].newLayout						= BuildImageLayout(Images[ImageIndex].After);
		ImageMemoryBarriers[ImageIndex].srcQueueFamilyIndex				= 0;
		ImageMemoryBarriers[ImageIndex].dstQueueFamilyIndex				= 0;
		ImageMemoryBarriers[ImageIndex].image							= CurrentVulkanBuffer->GetImage();
		ImageMemoryBarriers[ImageIndex].subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		ImageMemoryBarriers[ImageIndex].subresourceRange.baseMipLevel	= 0;
		ImageMemoryBarriers[ImageIndex].subresourceRange.levelCount		= CurrentVulkanBuffer->GetMipCount();
		ImageMemoryBarriers[ImageIndex].subresourceRange.baseArrayLayer	= 0;
		ImageMemoryBarriers[ImageIndex].subresourceRange.layerCount		= CurrentVulkanBuffer->GetDepth();
	}

	VkPipelineStageFlags PipelineStageFlagsBefore = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
		| VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
		| VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkPipelineStageFlags PipelineStageFlagsAfter = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
		| VK_PIPELINE_STAGE_VERTEX_SHADER_BIT
		| VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
		| VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	
	vkCmdPipelineBarrier(
		_CommandBuffer,
		PipelineStageFlagsBefore, PipelineStageFlagsAfter, VK_DEPENDENCY_BY_REGION_BIT,
		0, nullptr,
		BufferMemoryBarriers.size(), BufferMemoryBarriers.data(),
		ImageMemoryBarriers.size(), ImageMemoryBarriers.data()
	);
}
