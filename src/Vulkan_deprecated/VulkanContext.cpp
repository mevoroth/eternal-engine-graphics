#include "Vulkan_deprecated/VulkanContext.hpp"

#include "Vulkan_deprecated/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanContext::VulkanContext(_In_ Device& DeviceObj)
	: Context(DeviceObj)
{
	VkSemaphoreCreateInfo SemaphoreInfo;
	SemaphoreInfo.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	SemaphoreInfo.pNext	= nullptr;
	SemaphoreInfo.flags	= 0;

	VkResult Result = vkCreateSemaphore(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &SemaphoreInfo, nullptr, &_FrameSemaphore);
	ETERNAL_ASSERT(!Result);
}
