#include "Vulkan_deprecated/VulkanSampler.hpp"

#include <float.h>
#include "Vulkan_deprecated/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanSampler::VulkanSampler(_In_ Device& DeviceObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
	: Sampler(MINLinear, MAGLinear, MIPLinear, U, V, W)
{
	//VkSamplerCreateInfo SamplerInfo;
	//SamplerInfo.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	//SamplerInfo.pNext					= nullptr;
	//SamplerInfo.flags					= 0;
	//SamplerInfo.magFilter				= (MAGLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST);
	//SamplerInfo.minFilter				= (MINLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST);
	//SamplerInfo.mipmapMode				= (MIPLinear ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST);
	//SamplerInfo.addressModeU			= (VkSamplerAddressMode)(U - 1);
	//SamplerInfo.addressModeV			= (VkSamplerAddressMode)(V - 1);
	//SamplerInfo.addressModeW			= (VkSamplerAddressMode)(W - 1);
	//SamplerInfo.mipLodBias				= 0.0f;
	//SamplerInfo.anisotropyEnable		= VK_FALSE;
	//SamplerInfo.maxAnisotropy			= 1.0f;
	//SamplerInfo.compareEnable			= VK_FALSE;
	//SamplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
	//SamplerInfo.minLod					= 0.0f;
	//SamplerInfo.maxLod					= FLT_MAX;
	//SamplerInfo.borderColor				= VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	//SamplerInfo.unnormalizedCoordinates = VK_FALSE;

	//VkResult Result = vkCreateSampler(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &SamplerInfo, nullptr, &_Sampler);
	//ETERNAL_ASSERT(!Result);

	//VkDescriptorImageInfo DescriptorImageInfo;
	//DescriptorImageInfo.sampler

	//VkWriteDescriptorSet WriteDescriptorSet;
	//WriteDescriptorSet.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	//WriteDescriptorSet.pNext			= nullptr;
	//WriteDescriptorSet.dstSet			= 0;
	//WriteDescriptorSet.dstBinding		= 0;
	//WriteDescriptorSet.dstArrayElement	= 0;
	//WriteDescriptorSet.descriptorCount	= 0;
	//WriteDescriptorSet.descriptorType	= VK_DESCRIPTOR_TYPE_SAMPLER;
	//WriteDescriptorSet.pImageInfo		= 

	//VkStructureType                  sType;
	//const void*                      pNext;
	//VkDescriptorSet                  dstSet;
	//uint32_t                         dstBinding;
	//uint32_t                         dstArrayElement;
	//uint32_t                         descriptorCount;
	//VkDescriptorType                 descriptorType;
	//const VkDescriptorImageInfo*     pImageInfo;
	//const VkDescriptorBufferInfo*    pBufferInfo;
	//const VkBufferView*              pTexelBufferView;
}
