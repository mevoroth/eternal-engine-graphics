#include "Vulkan/VulkanCommandAllocator.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanCommandAllocator::VulkanCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue)
			: _Device(static_cast<VulkanDevice&>(InDevice))
		{
			vk::CommandPoolCreateInfo CommandPoolInfo(
				vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
				static_cast<const VulkanCommandQueue&>(InCommandQueue).GetQueueFamilyIndex()
			);

			Vulkan::VerifySuccess(
				_Device.GetVulkanDevice().createCommandPool(&CommandPoolInfo, nullptr, &_CommandPool)
			);
		}

		VulkanCommandAllocator::~VulkanCommandAllocator()
		{
			_Device.GetVulkanDevice().destroyCommandPool(_CommandPool);
		}

		void VulkanCommandAllocator::Reset()
		{
			_Device.GetVulkanDevice().resetCommandPool(
				_CommandPool,
				vk::CommandPoolResetFlagBits::eReleaseResources
			);
		}
	}
}
