#include "Vulkan/VulkanRootSignature.hpp"

#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanSampler.hpp"
#include "Vulkan/VulkanDevice.hpp"

using namespace std;

//namespace Eternal
//{
//	namespace Graphics
//	{
//		namespace Vulkan
//		{
//			static const vk::DescriptorType VULKAN_DESCRIPTOR_TYPES[] =
//			{
//				vk::DescriptorType::eSampler,
//				vk::DescriptorType::eSampledImage,
//				vk::DescriptorType::eStorageImage,
//				vk::DescriptorType::eUniformTexelBuffer,
//				vk::DescriptorType::eStorageTexelBuffer,
//				vk::DescriptorType::eUniformBuffer,
//				vk::DescriptorType::eStorageBuffer,
//				vk::DescriptorType::eUniformBufferDynamic,
//				vk::DescriptorType::eStorageBufferDynamic,
//				vk::DescriptorType::eInputAttachment,
//				vk::DescriptorType::eInputAttachment
//			};
//
//			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_DESCRIPTOR_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Vulkan Descriptor Types declaration not complete");
//		}
//	}
//
//}

//VulkanRootSignature::VulkanRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
//	: _Device(DeviceObj)
//	, _ParametersCount(ResourcesCount)
//{
//	using namespace Vulkan;
//
//	vk::Device& VkDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();
//
//	_DescriptorSetLayouts.resize(ResourcesCount);
//
//	uint32_t VkRegister = 0;
//	for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
//	{
//		const vector<RootSignatureParameter>& Parameter = Resources[ResourceIndex];
//		vector<vk::DescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
//		DescriptorSetLayoutBindings.resize(Parameter.size());
//		for (uint32_t BindingIndex = 0; BindingIndex < Parameter.size(); ++BindingIndex)
//		{
//			DescriptorSetLayoutBindings[BindingIndex] = vk::DescriptorSetLayoutBinding(
//				Parameter[BindingIndex].Register,
//				VULKAN_DESCRIPTOR_TYPES[int(Parameter[BindingIndex].Parameter)],
//				1,
//				ConvertRootSignatureAccessToShaderStageFlags(Parameter[BindingIndex].Access),
//				nullptr
//			);
//		}
//
//		vk::DescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo(
//			vk::DescriptorSetLayoutCreateFlagBits(),
//			DescriptorSetLayoutBindings.size(),
//			DescriptorSetLayoutBindings.data()
//		);
//
//		VerifySuccess(VkDeviceObj.createDescriptorSetLayout(&DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayouts[ResourceIndex]));
//	}
//
//	vk::PipelineLayoutCreateInfo PipelineLayoutInfo(
//		vk::PipelineLayoutCreateFlagBits(),
//		_DescriptorSetLayouts.size(),
//		_DescriptorSetLayouts.data()
//	);
//
//	VerifySuccess(VkDeviceObj.createPipelineLayout(&PipelineLayoutInfo, nullptr, &_PipelineLayout));
//}

namespace Eternal
{
	namespace Graphics
	{
		VulkanRootSignature::VulkanRootSignature(_In_ Device& InDevice)
			: RootSignature()
			, _Device(InDevice)
		{
			using namespace Vulkan;

			vk::Device& VkDevice = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();
			vk::DescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
			VerifySuccess(
				VkDevice.createDescriptorSetLayout(&DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout)
			);

			vk::PipelineLayoutCreateInfo PipelineLayoutInfo;
			VerifySuccess(
				VkDevice.createPipelineLayout(&PipelineLayoutInfo, nullptr, &_PipelineLayout)
			);
		}

		VulkanRootSignature::VulkanRootSignature(_In_ Device& InDevice, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
			: RootSignature(InRootSignatureCreateInformation)
			, _Device(InDevice)
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();
			//vk::Descset
		}

		VulkanRootSignature::~VulkanRootSignature()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(_Device).GetVulkanDevice();
			VkDevice.destroyDescriptorSetLayout(_DescriptorSetLayout);
			VkDevice.destroyPipelineLayout(_PipelineLayout);
		}
	}
}
