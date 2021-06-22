#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanGraphicsContext::VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: GraphicsContext(CreateInformation)
		{
			vk::SemaphoreCreateInfo SemaphoreInfo;

			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice();
			for (int32_t AcquireSemaphoreIndex = 0; AcquireSemaphoreIndex < _AcquireFrameSemaphores.size(); ++AcquireSemaphoreIndex)
			{
				Vulkan::VerifySuccess(
					VkDevice.createSemaphore(&SemaphoreInfo, nullptr, &_AcquireFrameSemaphores[AcquireSemaphoreIndex])
				);
			}
		}

		VulkanGraphicsContext::~VulkanGraphicsContext()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice();
			for (int32_t AcquireSemaphoreIndex = 0; AcquireSemaphoreIndex < _AcquireFrameSemaphores.size(); ++AcquireSemaphoreIndex)
			{
				VkDevice.destroySemaphore(_AcquireFrameSemaphores[AcquireSemaphoreIndex]);
			}
		}

		void VulkanGraphicsContext::UpdateGraphicsContext()
		{
			//CurrentFrameIndex = (CurrentFrameIndex + 1) % FrameBufferingCount;
		}
	}
}
