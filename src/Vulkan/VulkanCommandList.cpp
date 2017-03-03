#include "Vulkan/VulkanCommandList.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"

using namespace Eternal::Graphics;

VulkanCommandList::VulkanCommandList(_In_ VulkanDevice& Device, _In_ VulkanCommandQueue& CommandQueue)
	: _Device(Device)
	, _CommandQueue(CommandQueue)
{
	VkCommandBufferAllocateInfo CommandBufferAllocateInfo;
	CommandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferAllocateInfo.pNext					= nullptr;
	CommandBufferAllocateInfo.commandPool			= CommandQueue.GetCommandPool();
	CommandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferAllocateInfo.commandBufferCount	= Device.GetBackBufferFrameCount();

	VkResult Result = vkAllocateCommandBuffers(Device.GetDevice(), &CommandBufferAllocateInfo, &_CommandBuffer);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandList::~VulkanCommandList()
{
	vkFreeCommandBuffers(_Device.GetDevice(), _CommandQueue.GetCommandPool(), 1, &_CommandBuffer);
	_CommandBuffer = nullptr;
}

void VulkanCommandList::Begin()
{
}

void VulkanCommandList::End()
{
}

void VulkanCommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
}
