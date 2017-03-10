#include "Vulkan/VulkanCommandAllocator.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanCommandAllocator::VulkanCommandAllocator(_In_ VulkanDevice& DeviceObj)
	: _Device(DeviceObj)
{
	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext				= nullptr;
	CommandPoolInfo.flags				= 0;
	CommandPoolInfo.queueFamilyIndex	= DeviceObj.GetQueueFamilyIndex();

	VkResult Result = vkCreateCommandPool(DeviceObj.GetDevice(), &CommandPoolInfo, nullptr, &_CommandPool);
	ETERNAL_ASSERT(!Result);

	VkSemaphoreCreateInfo SemaphoreInfo;

	SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	SemaphoreInfo.pNext = nullptr;
	SemaphoreInfo.flags = 0;

	Result = vkCreateSemaphore(DeviceObj.GetDevice(), &SemaphoreInfo, nullptr, &_CompletedSemaphore);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandAllocator::~VulkanCommandAllocator()
{
	vkDestroySemaphore(_Device.GetDevice(), _CompletedSemaphore, nullptr);
	vkDestroyCommandPool(_Device.GetDevice(), _CommandPool, nullptr);
}
