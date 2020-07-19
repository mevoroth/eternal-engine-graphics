#include "Vulkan/VulkanFence.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ Device& DeviceObj)
	: _Device(DeviceObj)
{
	//VkFenceCreateInfo FenceInfo;
	//FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//FenceInfo.pNext = nullptr;
	//FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	//VkResult Result = vkCreateFence(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &FenceInfo, nullptr, &_Fence);
	//ETERNAL_ASSERT(!Result);

	//Result = vkResetFences(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), 1u, &_Fence);
	//ETERNAL_ASSERT(!Result);
}

VulkanFence::~VulkanFence()
{
	vkDestroyFence(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _Fence, nullptr);
}

void VulkanFence::Signal(_In_ CommandQueue& CommandQueueObj)
{
}

void VulkanFence::Wait(_In_ Device& DeviceObj)
{
	//VkDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();
	//VkResult Result;
	//Result = vkGetFenceStatus(VulkanDeviceObj, _Fence);
	//if (Result == VK_SUCCESS)
	//{
	//	Result = vkWaitForFences(VulkanDeviceObj, 1, &_Fence, VK_TRUE, UINT64_MAX);
	//	ETERNAL_ASSERT(!Result);
	//}
	////else
	////{
	////	ETERNAL_ASSERT(false);
	////}
}

void VulkanFence::Reset(_In_ Device& DeviceObj)
{
	//VkResult Result = vkResetFences(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), 1, &_Fence);
	//ETERNAL_ASSERT(!Result);
}

VkFence_T* VulkanFence::GetVulkanFence()
{
	return _Fence;
}
