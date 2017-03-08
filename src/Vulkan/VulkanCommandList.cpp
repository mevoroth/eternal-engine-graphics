#include "Vulkan/VulkanCommandList.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
<<<<<<< Updated upstream
=======
#include "Vulkan/VulkanState.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#include "Vulkan/VulkanFrameBuffer.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Graphics/Viewport.hpp"
>>>>>>> Stashed changes

using namespace Eternal::Graphics;

VulkanCommandList::VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanCommandQueue& CommandQueueObj)
	: _Device(DeviceObj)
	, _CommandQueue(CommandQueueObj)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo;
	CommandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.pNext					= nullptr;
	CommandBufferAllocateInfo.commandPool			= CommandQueueObj.GetCommandPool();
	CommandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount	= DeviceObj.GetBackBufferFrameCount();

	VkResult Result = vkAllocateCommandBuffers(DeviceObj.GetDevice(), &CommandBufferAllocateInfo, &_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandList::~VulkanCommandList()
{
	vkFreeCommandBuffers(_Device.GetDevice(), _CommandQueue.GetCommandPool(), 1, &_CommandBuffer);
	_CommandBuffer = nullptr;
}

<<<<<<< Updated upstream
void VulkanCommandList::Begin()
{
=======
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
>>>>>>> Stashed changes
}

void VulkanCommandList::End()
{
<<<<<<< Updated upstream
=======
	vkCmdEndRenderPass(_CommandBuffer);
	VkResult Result = vkEndCommandBuffer(_CommandBuffer);
	ETERNAL_ASSERT(!Result);
>>>>>>> Stashed changes
}

void VulkanCommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
}
