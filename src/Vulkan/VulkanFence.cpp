#include "Vulkan/VulkanFence.hpp"

#include "Macros/Macros.hpp"

#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount)
	: _Device(DeviceObj)
{
	ETERNAL_ASSERT(SimultaneousResourcesCount > 0);

	_Fences.resize(SimultaneousResourcesCount);

	for (uint32_t ResourceIndex = 0; ResourceIndex < SimultaneousResourcesCount; ++ResourceIndex)
	{
		VkFenceCreateInfo FenceInfo;
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.pNext = nullptr;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult Result = vkCreateFence(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &FenceInfo, nullptr, &_Fences[ResourceIndex]);
		ETERNAL_ASSERT(!Result);
	}

	VkResult Result = vkResetFences(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), (uint32_t)_Fences.size(), _Fences.data());
	ETERNAL_ASSERT(!Result);
}

VulkanFence::~VulkanFence()
{
	for (uint32_t ResourceIndex = 0; ResourceIndex < _Fences.size(); ++ResourceIndex)
	{
		vkDestroyFence(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _Fences[ResourceIndex], nullptr);
	}
}

void VulkanFence::Signal(_In_ CommandQueue& CommandQueueObj)
{
	_FenceIndex = (_FenceIndex + 1) % _Fences.size();
}

void VulkanFence::Wait(_In_ Device& DeviceObj)
{
	VkDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();
	VkResult Result;
	Result = vkGetFenceStatus(VulkanDeviceObj, _Fences[_FenceIndex]);
	if (Result == VK_SUCCESS)
	{
		Result = vkWaitForFences(VulkanDeviceObj, 1, &_Fences[_FenceIndex], VK_TRUE, UINT64_MAX);
		ETERNAL_ASSERT(!Result);
	}
}

void VulkanFence::Reset(_In_ Device& DeviceObj)
{
	VkResult Result = vkResetFences(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), 1, &_Fences[_FenceIndex]);
	ETERNAL_ASSERT(!Result);
}

VkFence_T* VulkanFence::GetFence()
{
	return _Fences[_FenceIndex];
}
