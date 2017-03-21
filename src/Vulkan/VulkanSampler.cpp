#include "Vulkan/VulkanSampler.hpp"

#include <float.h>
#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanSampler::VulkanSampler(_In_ Device& DeviceObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
{
	VkSamplerCreateInfo SamplerInfo;
	SamplerInfo.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.pNext					= nullptr;
	SamplerInfo.flags					= 0;
	SamplerInfo.magFilter				= (MAGLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST);
	SamplerInfo.minFilter				= (MINLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST);
	SamplerInfo.mipmapMode				= (MIPLinear ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST);
	SamplerInfo.addressModeU			= (VkSamplerAddressMode)(U - 1);
	SamplerInfo.addressModeV			= (VkSamplerAddressMode)(V - 1);
	SamplerInfo.addressModeW			= (VkSamplerAddressMode)(W - 1);
	SamplerInfo.mipLodBias				= 0.0f;
	SamplerInfo.anisotropyEnable		= VK_FALSE;
	SamplerInfo.maxAnisotropy			= 0.0f;
	SamplerInfo.compareEnable			= VK_FALSE;
	SamplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
	SamplerInfo.minLod					= 0.0f;
	SamplerInfo.maxLod					= FLT_MAX;
	SamplerInfo.borderColor				= VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	SamplerInfo.unnormalizedCoordinates = VK_FALSE;

	VkResult Result = vkCreateSampler(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &SamplerInfo, nullptr, &_Sampler);
	ETERNAL_ASSERT(!Result);
}
