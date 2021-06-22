#include "Vulkan/VulkanCommandAllocator.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanUtils.hpp"

using namespace Eternal::Graphics;

VulkanCommandAllocator::VulkanCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue)
	: _Device(InDevice)
{
	vk::Device& VkDevice = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();

	vk::CommandPoolCreateInfo CommandPoolInfo(
		vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		static_cast<const VulkanCommandQueue&>(InCommandQueue).GetQueueFamilyIndex()
	);

	Vulkan::VerifySuccess(
		VkDevice.createCommandPool(&CommandPoolInfo, nullptr, &_CommandPool)
	);
}

VulkanCommandAllocator::~VulkanCommandAllocator()
{
	vk::Device& VkDevice = static_cast<VulkanDevice&>(_Device).GetVulkanDevice();
	VkDevice.destroyCommandPool(_CommandPool);
}
