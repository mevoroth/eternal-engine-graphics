#include "Vulkan/VulkanCommandQueue.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanCommandQueue::VulkanCommandQueue(_In_ VulkanDevice& Device)
	: _Device(Device)
{
	vkGetDeviceQueue(_Device.GetDevice(), 0, 0, &_CommandQueue);

	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext				= nullptr;
	CommandPoolInfo.flags				= 0;
	CommandPoolInfo.queueFamilyIndex	= Device.GetQueueFamilyIndex();

	VkResult Result = vkCreateCommandPool(Device.GetDevice(), &CommandPoolInfo, nullptr, &_CommandPool);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandQueue::~VulkanCommandQueue()
{
	vkDestroyCommandPool(_Device.GetDevice(), _CommandPool, nullptr);
	_CommandPool = nullptr;
}

void VulkanCommandQueue::Reset(_In_ uint32_t FrameIndex)
{

}
