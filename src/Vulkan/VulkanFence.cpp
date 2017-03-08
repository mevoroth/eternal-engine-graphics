#include "Vulkan/VulkanFence.hpp"

#include "Macros/Macros.hpp"

#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ VulkanDevice& DeviceObj)
	: _Device(DeviceObj)
{
	VkFenceCreateInfo FenceInfo;
	FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceInfo.pNext = nullptr;
	FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkResult Result = vkCreateFence(DeviceObj.GetDevice(), &FenceInfo, nullptr, &_Fence);
	ETERNAL_ASSERT(!Result);
}

VulkanFence::~VulkanFence()
{
	vkDestroyFence(_Device.GetDevice(), _Fence, nullptr);
}
