#include "Vulkan/VulkanFence.hpp"

#include "Macros/Macros.hpp"

#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ VulkanDevice& Device)
{
	VkFenceCreateInfo FenceInfo;
	FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	FenceInfo.pNext = nullptr;
	FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkResult Result = vkCreateFence(Device.GetDevice(), &FenceInfo, nullptr, &_Fence);
	ETERNAL_ASSERT(!Result);
}
