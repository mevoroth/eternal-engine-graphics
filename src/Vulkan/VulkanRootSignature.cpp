#include "Vulkan/VulkanRootSignature.hpp"

#include <vector>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanSampler.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanDescriptorHeap.hpp"

using namespace std;
using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		const VkDescriptorType VULKAN_DESCRIPTOR_TYPES[] =
		{
			VK_DESCRIPTOR_TYPE_SAMPLER,
			VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
			VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
			VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
		};
	}

	ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_DESCRIPTOR_TYPES) == ROOT_SIGNATURE_PARAMETER_COUNT, "Vulkan Descriptor Types declaration not complete");
}

VulkanRootSignature::VulkanRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	//ETERNAL_ASSERT((StaticSamplersCount + ParametersCount) > 0);

	VkDevice& VkDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();

	uint32_t BindingIndex = 0;
	vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
	DescriptorSetLayoutBindings.resize(/*ParametersCount +*/ (StaticSamplersCount ? 1 : 0));

	// Static samplers
	vector<VkSampler> VulkanStaticSamplers;
	if (StaticSamplersCount)
	{
		VulkanStaticSamplers.resize(StaticSamplersCount);
		for (uint32_t SamplerIndex = 0; SamplerIndex < StaticSamplersCount; ++SamplerIndex)
		{
			VulkanStaticSamplers[SamplerIndex] = static_cast<VulkanSampler*>(StaticSamplers[SamplerIndex])->GetVulkanSampler();
		}
		DescriptorSetLayoutBindings[BindingIndex].binding				= BindingIndex;
		DescriptorSetLayoutBindings[BindingIndex].descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DescriptorSetLayoutBindings[BindingIndex].descriptorCount		= StaticSamplersCount;
		DescriptorSetLayoutBindings[BindingIndex].stageFlags			= 0xFFFFFFFF;//RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
		DescriptorSetLayoutBindings[BindingIndex].pImmutableSamplers	= VulkanStaticSamplers.data();
		
		BindingIndex++;
	}

	//for (uint32_t ParameterIndex = 0; ParameterIndex < ParametersCount; ++ParameterIndex)
	//{
	//	DescriptorSetLayoutBindings[BindingIndex].binding				= BindingIndex;
	//	DescriptorSetLayoutBindings[BindingIndex].descriptorType		= VULKAN_DESCRIPTOR_TYPES[Parameters[ParameterIndex].Type];
	//	DescriptorSetLayoutBindings[BindingIndex].descriptorCount		= Parameters[ParameterIndex].Count;
	//	DescriptorSetLayoutBindings[BindingIndex].stageFlags			= RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
	//	DescriptorSetLayoutBindings[BindingIndex].pImmutableSamplers	= nullptr;
	//}

	//VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	//DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	//DescriptorSetLayoutInfo.pNext			= nullptr;
	//DescriptorSetLayoutInfo.flags			= 0;
	//DescriptorSetLayoutInfo.bindingCount	= DescriptorSetLayoutBindings.size();
	//DescriptorSetLayoutInfo.pBindings		= DescriptorSetLayoutBindings.data();

	//VkResult Result = vkCreateDescriptorSetLayout(VkDeviceObj, &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	//ETERNAL_ASSERT(!Result);

	//vector<VkDescriptorSetLayout> DescriptorsSetLayouts;
	//DescriptorsSetLayouts.resize(1 + RootSignaturesCount);

	//for (uint32_t DescriptorSetLayoutIndex = 0; DescriptorSetLayoutIndex < RootSignaturesCount; ++DescriptorSetLayoutIndex)
	//{
	//	DescriptorsSetLayouts[DescriptorSetLayoutIndex] = static_cast<VulkanRootSignature*>(RootSignatures[DescriptorSetLayoutIndex])->GetDescriptorSetLayout();
	//}
	//DescriptorsSetLayouts[RootSignaturesCount] = _DescriptorSetLayout;

	vector<VkDescriptorSetLayout> DescriptorsSetLayouts;
	DescriptorsSetLayouts.resize(DescriptorHeapsCount);

	for (unsigned int DescriptorSetIndex = 0; DescriptorSetIndex < DescriptorsSetLayouts.size(); ++DescriptorSetIndex)
	{
		DescriptorsSetLayouts[DescriptorSetIndex] = static_cast<VulkanDescriptorHeap*>(DescriptorHeaps[DescriptorSetIndex])->GetDescriptorSetLayout();
	}

	VkPipelineLayoutCreateInfo PipelineLayoutInfo;
	PipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.pNext					= nullptr;
	PipelineLayoutInfo.flags					= 0;
	PipelineLayoutInfo.setLayoutCount			= DescriptorHeapsCount;
	PipelineLayoutInfo.pSetLayouts				= DescriptorsSetLayouts.data();
	PipelineLayoutInfo.pushConstantRangeCount	= 0;
	PipelineLayoutInfo.pPushConstantRanges		= nullptr;

	VkResult Result = vkCreatePipelineLayout(VkDeviceObj, &PipelineLayoutInfo, nullptr, &_PipelineLayout);
	ETERNAL_ASSERT(!Result);
}

VulkanRootSignature::VulkanRootSignature(_In_ Device& DeviceObj)
{
	VkDevice& VkDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();

	//VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	//DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	//DescriptorSetLayoutInfo.pNext			= nullptr;
	//DescriptorSetLayoutInfo.flags			= 0;
	//DescriptorSetLayoutInfo.bindingCount	= 0;
	//DescriptorSetLayoutInfo.pBindings		= nullptr;

	//VkResult Result = vkCreateDescriptorSetLayout(VkDeviceObj, &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	//ETERNAL_ASSERT(!Result);

	VkPipelineLayoutCreateInfo PipelineLayoutInfo;
	PipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.pNext					= nullptr;
	PipelineLayoutInfo.flags					= 0;
	PipelineLayoutInfo.setLayoutCount			= 0;
	PipelineLayoutInfo.pSetLayouts				= nullptr;
	PipelineLayoutInfo.pushConstantRangeCount	= 0;
	PipelineLayoutInfo.pPushConstantRanges		= nullptr;

	VkResult Result = vkCreatePipelineLayout(VkDeviceObj, &PipelineLayoutInfo, nullptr, &_PipelineLayout);
	ETERNAL_ASSERT(!Result);
}
