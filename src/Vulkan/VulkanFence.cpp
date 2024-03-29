#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanFence.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanFence::VulkanFence(_In_ Device& InDevice)
			: _Device(static_cast<VulkanDevice&>(InDevice))
		{
			vk::FenceCreateInfo FenceInfo(
				vk::FenceCreateFlagBits::eSignaled
			);
			vk::Device& VkDevice = _Device.GetVulkanDevice();
			Vulkan::VerifySuccess(
				VkDevice.createFence(&FenceInfo, nullptr, &_Fence)
			);
		}

		VulkanFence::~VulkanFence()
		{
			_Device.GetVulkanDevice().destroyFence(_Fence);
		}

		void VulkanFence::Wait(_In_ Device& InDevice)
		{
			Vulkan::VerifySuccess(
				_Device.GetVulkanDevice().waitForFences(
					1, &_Fence, true, UINT64_MAX
				)
			);
		}

		void VulkanFence::Reset(_In_ Device& InDevice)
		{
			Vulkan::VerifySuccess(
				_Device.GetVulkanDevice().resetFences(
					1, &_Fence
				)
			);
		}
	}
}

#endif
