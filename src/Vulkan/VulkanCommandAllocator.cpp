#include "Vulkan/VulkanCommandAllocator.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanCommandAllocator::VulkanCommandAllocator(_In_ Device& DeviceObj)
	: _Device(DeviceObj)
{
	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj);

	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext				= nullptr;
	CommandPoolInfo.flags				= 0;
	CommandPoolInfo.queueFamilyIndex	= VulkanDeviceObj.GetQueueFamilyIndex();

	VkResult Result = vkCreateCommandPool(VulkanDeviceObj.GetVulkanDevice(), &CommandPoolInfo, nullptr, &_CommandPool);
	ETERNAL_ASSERT(!Result);

	VkSemaphoreCreateInfo SemaphoreInfo;

	SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	SemaphoreInfo.pNext = nullptr;
	SemaphoreInfo.flags = 0;

	Result = vkCreateSemaphore(VulkanDeviceObj.GetVulkanDevice(), &SemaphoreInfo, nullptr, &_CompletedSemaphore);
	ETERNAL_ASSERT(!Result);
}

VulkanCommandAllocator::~VulkanCommandAllocator()
{
	VkDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(_Device).GetVulkanDevice();
	vkDestroySemaphore(VulkanDeviceObj, _CompletedSemaphore, nullptr);
	vkDestroyCommandPool(VulkanDeviceObj, _CommandPool, nullptr);
}
