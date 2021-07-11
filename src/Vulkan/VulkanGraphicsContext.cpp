#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanDescriptorTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		VulkanGraphicsContext::VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: GraphicsContext(CreateInformation)
			, _ConstantHandles(static_cast<VulkanDevice&>(GetDevice()).GetPushConstantMaxSize())
		{
			vk::SemaphoreCreateInfo SemaphoreInfo;

			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice();
			for (int32_t AcquireSemaphoreIndex = 0; AcquireSemaphoreIndex < _AcquireFrameSemaphores.size(); ++AcquireSemaphoreIndex)
			{
				VerifySuccess(
					VkDevice.createSemaphore(&SemaphoreInfo, nullptr, &_AcquireFrameSemaphores[AcquireSemaphoreIndex])
				);
			}

			vk::DescriptorPoolSize PoolSizes[] =
			{
				vk::DescriptorPoolSize(vk::DescriptorType::eSampler,			MaxSamplersDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage,		MaxSampledImageDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage,		MaxStorageImageDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer,	MaxUniformTexelBufferDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer,	MaxStorageTexelBufferDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer,		MaxUniformBufferDescriptorCount),
				vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer,		MaxStorageBufferDescriptorCount)
			};

			vk::DescriptorPoolCreateInfo DescriptorPoolCreateInfo(
				vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
				MaxDescriptorSetsCount,
				ETERNAL_ARRAYSIZE(PoolSizes),
				PoolSizes
			);
			VerifySuccess(
				VkDevice.createDescriptorPool(
					&DescriptorPoolCreateInfo,
					nullptr,
					&_DescriptorPool
				)
			);
		}

		VulkanGraphicsContext::~VulkanGraphicsContext()
		{
			WaitForAllFences();

			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice();
			for (int32_t AcquireSemaphoreIndex = 0; AcquireSemaphoreIndex < _AcquireFrameSemaphores.size(); ++AcquireSemaphoreIndex)
			{
				VkDevice.destroySemaphore(_AcquireFrameSemaphores[AcquireSemaphoreIndex]);
			}
		}

		void VulkanGraphicsContext::ResetFrameStates()
		{
			GraphicsContext::ResetFrameStates();

			for (uint32_t DescriptorTableIndex = 0; DescriptorTableIndex < _DescriptorTables.size(); ++DescriptorTableIndex)
				_DescriptorTables[DescriptorTableIndex]->Reset(GetCurrentFrameIndex());
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
