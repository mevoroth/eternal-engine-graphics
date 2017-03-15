#include "Vulkan/VulkanRootSignature.hpp"

#include <vector>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanSampler.hpp"
#include "Vulkan/VulkanDevice.hpp"

using namespace std;
using namespace Eternal::Graphics;

static const VkShaderStageFlags RootSignatureAccessToVkShaderStageFlags(_In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	return	(RootSignatureAccessObj & VS) ? VK_SHADER_STAGE_VERTEX_BIT					: 0
		|	(RootSignatureAccessObj & HS) ? VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT	: 0
		|	(RootSignatureAccessObj & DS) ? VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT	: 0
		|	(RootSignatureAccessObj & GS) ? VK_SHADER_STAGE_GEOMETRY_BIT				: 0
		|	(RootSignatureAccessObj & PS) ? VK_SHADER_STAGE_COMPUTE_BIT					: 0;
}

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
	VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
};

VulkanRootSignature::VulkanRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ const RootSignatureDynamicParameter Parameters[], _In_ uint32_t ParametersCount, _In_ const RootSignature RootSignatures[], _In_ uint32_t RootSignaturesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	uint32_t TotalDescriptorSetLayouts = StaticSamplersCount + ParametersCount + RootSignaturesCount;
	ETERNAL_ASSERT(TotalDescriptorSetLayouts > 0);

	VkDevice& VkDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();

	uint32_t BindingIndex = 0;
	vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
	DescriptorSetLayoutBindings.resize(ParametersCount + (StaticSamplersCount ? 1 : 0));

	// Static samplers
	vector<VkSampler> VulkanStaticSamplers;
	if (StaticSamplersCount)
	{
		VulkanStaticSamplers.resize(StaticSamplersCount);
		for (uint32_t SamplerIndex; SamplerIndex < StaticSamplersCount; ++SamplerIndex)
		{
			VulkanStaticSamplers[SamplerIndex] = static_cast<VulkanSampler*>(StaticSamplers[SamplerIndex])->GetVulkanSampler();
		}
		DescriptorSetLayoutBindings[BindingIndex].binding				= BindingIndex;
		DescriptorSetLayoutBindings[BindingIndex].descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DescriptorSetLayoutBindings[BindingIndex].descriptorCount		= StaticSamplersCount;
		DescriptorSetLayoutBindings[BindingIndex].stageFlags			= RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
		DescriptorSetLayoutBindings[BindingIndex].pImmutableSamplers	= VulkanStaticSamplers.data();
		
		BindingIndex++;
	}

	for (uint32_t ParameterIndex = 0; ParameterIndex < ParametersCount; ++ParameterIndex)
	{
		DescriptorSetLayoutBindings[BindingIndex].binding				= BindingIndex;
		DescriptorSetLayoutBindings[BindingIndex].descriptorType		= VULKAN_DESCRIPTOR_TYPES[Parameters[ParameterIndex].Type];
		DescriptorSetLayoutBindings[BindingIndex].descriptorCount		= Parameters[ParameterIndex].Count;
		DescriptorSetLayoutBindings[BindingIndex].stageFlags			= RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
		DescriptorSetLayoutBindings[BindingIndex].pImmutableSamplers	= nullptr;
	}

	VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorSetLayoutInfo.pNext			= nullptr;
	DescriptorSetLayoutInfo.flags			= 0;
	DescriptorSetLayoutInfo.bindingCount	= DescriptorSetLayoutBindings.size();
	DescriptorSetLayoutInfo.pBindings		= DescriptorSetLayoutBindings.data();

	VkResult Result = vkCreateDescriptorSetLayout(VkDeviceObj, &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	ETERNAL_ASSERT(!Result);

	if (TotalDescriptorSetLayouts > StaticSamplersCount)
	{
		vector<VkDescriptorSetLayout> DescriptorsSetLayouts;
		DescriptorsSetLayouts.resize(1 + TotalDescriptorSetLayouts);

		VkPipelineLayoutCreateInfo PipelineLayoutInfo;
		PipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		PipelineLayoutInfo.pNext					= nullptr;
		PipelineLayoutInfo.flags					= 0;
		PipelineLayoutInfo.setLayoutCount			= DescriptorsSetLayouts.size();
		PipelineLayoutInfo.pSetLayouts				= DescriptorsSetLayouts.data();
		PipelineLayoutInfo.pushConstantRangeCount	= 0;
		PipelineLayoutInfo.pPushConstantRanges		= nullptr;

		Result = vkCreatePipelineLayout(VkDeviceObj, &PipelineLayoutInfo, nullptr, &_PipelineLayout);
		ETERNAL_ASSERT(!Result);
	}
}
