#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanGraphicsContext::VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: GraphicsContext(CreateInformation)
			, _ConstantHandles(static_cast<VulkanDevice&>(GetDevice()).GetPushConstantMaxSize())
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

		void VulkanGraphicsContext::AllocateConstantHandles(_In_ uint32_t ConstantCount, _Out_ vector<Handle>& OutHandles)
		{
			OutHandles.resize(ConstantCount);
			for (uint32_t ConstantIndex = 0; ConstantIndex < ConstantCount; ++ConstantIndex)
				OutHandles[ConstantIndex] = _ConstantHandles.Pop();
		}

		void VulkanGraphicsContext::ReleaseConstantHandles(_Inout_ vector<Handle>& InOutHandles)
		{
			for (uint32_t HandleIndex = 0; HandleIndex < InOutHandles.size(); ++HandleIndex)
				_ConstantHandles.Push(InOutHandles[HandleIndex]);
		}
	}
}
