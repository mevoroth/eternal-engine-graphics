#include "Vulkan/VulkanCommandList.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Graphics/Viewport.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include "Vulkan/VulkanRenderTarget.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanRootSignature.hpp"

using namespace Eternal::Graphics;

VulkanCommandList::VulkanCommandList(_In_ Device& DeviceObj, _In_ CommandAllocator& CommandAllocatorObj)
	: _Device(DeviceObj)
	, _CommandAllocator(CommandAllocatorObj)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo;
	CommandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.pNext					= nullptr;
	CommandBufferAllocateInfo.commandPool			= static_cast<VulkanCommandAllocator&>(CommandAllocatorObj).GetVulkanCommandPool();
	CommandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount	= 1;

	VkResult Result = vkAllocateCommandBuffers(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &CommandBufferAllocateInfo, &_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandList::~VulkanCommandList()
{
	vkFreeCommandBuffers(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), static_cast<VulkanCommandAllocator&>(_CommandAllocator).GetVulkanCommandPool(), 1, &_CommandBuffer);
	_CommandBuffer = nullptr;
}

void VulkanCommandList::Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj)
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

void VulkanCommandList::Begin(_In_ RenderTarget& FrameBufferObj, _In_ VulkanPipeline& PipelineObj, _In_ VulkanRenderPass& RenderPassObj)
{
	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	CommandBufferBeginInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.pNext			= nullptr;
	CommandBufferBeginInfo.flags			= 0;
	CommandBufferBeginInfo.pInheritanceInfo = nullptr;

	VkResult Result = vkBeginCommandBuffer(_CommandBuffer, &CommandBufferBeginInfo);
	ETERNAL_ASSERT(!Result);

	vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineObj.GetVulkanPipeline());

	VkClearValue ClearValue;
	ClearValue.color.float32[0] = 0.0f;
	ClearValue.color.float32[1] = 0.0f;
	ClearValue.color.float32[2] = 0.0f;
	ClearValue.color.float32[3] = 0.0f;

	VkRenderPassBeginInfo RenderPassBeginInfo;
	RenderPassBeginInfo.sType						= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.pNext						= nullptr;
	RenderPassBeginInfo.renderPass					= RenderPassObj.GetRenderPass();
	RenderPassBeginInfo.framebuffer					= static_cast<VulkanRenderTarget&>(FrameBufferObj).GetFrameBuffer();
	// FIX THIS
	RenderPassBeginInfo.renderArea.offset.x			= 0;
	RenderPassBeginInfo.renderArea.offset.y			= 0;
	RenderPassBeginInfo.renderArea.extent.width		= 1280;
	RenderPassBeginInfo.renderArea.extent.height	= 720;
	RenderPassBeginInfo.clearValueCount				= 1;
	RenderPassBeginInfo.pClearValues				= &ClearValue;

	vkCmdBeginRenderPass(_CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	//vkCmdPipelineBarrier()
	//VkCommandBuffer                             commandBuffer,
	//VkPipelineStageFlags                        srcStageMask,
	//VkPipelineStageFlags                        dstStageMask,
	//VkDependencyFlags                           dependencyFlags,
	//uint32_t                                    memoryBarrierCount,
	//const VkMemoryBarrier*                      pMemoryBarriers,
	//uint32_t                                    bufferMemoryBarrierCount,
	//const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
	//uint32_t                                    imageMemoryBarrierCount,
	//const VkImageMemoryBarrier*                 pImageMemoryBarriers);

	//VkPipelineStageFlags
	//VkDependencyFlags
	//VkBufferMemoryBarrier
}

void VulkanCommandList::End()
{
	vkCmdEndRenderPass(_CommandBuffer);
	VkResult Result = vkEndCommandBuffer(_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

void VulkanCommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
	vkCmdDraw(_CommandBuffer, PrimitiveCount, 1, 0, 0);
}

void VulkanCommandList::SetViewport(_In_ Viewport& ViewportObj)
{
	VkViewport VulkanViewport;
	VulkanViewport.x		= ViewportObj.X();
	VulkanViewport.y		= ViewportObj.Y();
	VulkanViewport.width	= ViewportObj.Width();
	VulkanViewport.height	= ViewportObj.Height();
	VulkanViewport.minDepth	= 0.0f;
	VulkanViewport.maxDepth	= 1.0f;

	vkCmdSetViewport(_CommandBuffer, 0, 1, &VulkanViewport);
}

void VulkanCommandList::SetScissorRectangle(_In_ Viewport& ViewportObj)
{
	VkRect2D VulkanScissor;
	VulkanScissor.offset.x		= ViewportObj.X();
	VulkanScissor.offset.y		= ViewportObj.Y();
	VulkanScissor.extent.width	= ViewportObj.Width();
	VulkanScissor.extent.height = ViewportObj.Height();

	vkCmdSetScissor(_CommandBuffer, 0, 1, &VulkanScissor);
}

void VulkanCommandList::BindPipelineInput(_In_ RootSignature& RootSignatureObj)
{
	ETERNAL_ASSERT(false);
	vkCmdBindDescriptorSets(
		_CommandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		static_cast<VulkanRootSignature&>(RootSignatureObj).GetPipelineLayout(),
		0,
		0,
		nullptr,
		0,
		nullptr
	);
}
