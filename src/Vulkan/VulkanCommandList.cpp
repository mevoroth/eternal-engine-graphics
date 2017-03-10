#include "Vulkan/VulkanCommandList.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanState.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include "Vulkan/VulkanFrameBuffer.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Graphics/Viewport.hpp"

using namespace Eternal::Graphics;

VulkanCommandList::VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj, _In_ VulkanCommandQueue& CommandQueueObj)
	: _Device(DeviceObj)
	, _CommandQueue(CommandQueueObj)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo;
	CommandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.pNext					= nullptr;
	CommandBufferAllocateInfo.commandPool			= CommandQueueObj.GetCommandPool();
	CommandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount	= SwapChainObj.GetBackBuffersFrameCount();

	VkResult Result = vkAllocateCommandBuffers(DeviceObj.GetVulkanDevice(), &CommandBufferAllocateInfo, &_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandList::~VulkanCommandList()
{
	vkFreeCommandBuffers(_Device.GetVulkanDevice(), _CommandQueue.GetCommandPool(), 1, &_CommandBuffer);
	_CommandBuffer = nullptr;
}

void VulkanCommandList::Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline, _In_ VulkanRenderPass& RenderPassObj)
{
	VkCommandBufferBeginInfo CommandBufferBeginInfo;
	CommandBufferBeginInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	CommandBufferBeginInfo.pNext			= nullptr;
	CommandBufferBeginInfo.flags			= 0;
	CommandBufferBeginInfo.pInheritanceInfo = nullptr;

	VkResult Result = vkBeginCommandBuffer(_CommandBuffer, &CommandBufferBeginInfo);
	ETERNAL_ASSERT(!Result);

	vkCmdBindPipeline(_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, State.GetPipeline());

	VkClearValue ClearValue;
	ClearValue.color.float32[0] = 0.0f;
	ClearValue.color.float32[1] = 0.0f;
	ClearValue.color.float32[2] = 0.0f;
	ClearValue.color.float32[3] = 0.0f;

	VkRenderPassBeginInfo RenderPassBeginInfo;
	RenderPassBeginInfo.sType						= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.pNext						= nullptr;
	RenderPassBeginInfo.renderPass					= RenderPassObj.GetRenderPass();
	RenderPassBeginInfo.framebuffer					= FrameBufferObj.GetFrameBuffer();
	// FIX THIS
	RenderPassBeginInfo.renderArea.offset.x			= 0;
	RenderPassBeginInfo.renderArea.offset.y			= 0;
	RenderPassBeginInfo.renderArea.extent.width		= 1280;
	RenderPassBeginInfo.renderArea.extent.height	= 720;
	RenderPassBeginInfo.clearValueCount				= 1;
	RenderPassBeginInfo.pClearValues				= &ClearValue;

	vkCmdBeginRenderPass(_CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
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
