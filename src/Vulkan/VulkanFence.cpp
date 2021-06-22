#include "Vulkan/VulkanFence.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtils.hpp"

using namespace Eternal::Graphics;

VulkanFence::VulkanFence(_In_ Device& InDevice)
	: _Device(InDevice)
{
	vk::FenceCreateInfo FenceInfo(
		vk::FenceCreateFlagBits::eSignaled
	);
	vk::Device& VkDevice = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();
	Vulkan::VerifySuccess(
		VkDevice.createFence(&FenceInfo, nullptr, &_Fence)
	);
}

VulkanFence::~VulkanFence()
{
	vk::Device& VkDevice = static_cast<VulkanDevice&>(_Device).GetVulkanDevice();
	VkDevice.destroyFence(_Fence);
}

void VulkanFence::Wait(_In_ Device& InDevice)
{
	Vulkan::VerifySuccess(
		static_cast<VulkanDevice&>(InDevice).GetVulkanDevice().waitForFences(
			1, &_Fence, true, UINT64_MAX
		)
	);
}

void VulkanFence::Reset(_In_ Device& InDevice)
{
	Vulkan::VerifySuccess(
		static_cast<VulkanDevice&>(InDevice).GetVulkanDevice().resetFences(
			1, &_Fence
		)
	);
}
