#include "Vulkan/VulkanDescriptorHeap.hpp"

#include <vector>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/RootSignature.hpp"

using namespace std;
using namespace Eternal::Graphics;

extern const VkDescriptorType VULKAN_DESCRIPTOR_TYPES[];

VulkanDescriptorHeap::VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount)
{
	VkDescriptorPoolSize DescriptorPoolSize;
	DescriptorPoolSize.type				= VULKAN_DESCRIPTOR_TYPES[HeapTypeObj];
	DescriptorPoolSize.descriptorCount	= ResourcesCount;

	VkDescriptorPoolCreateInfo DescriptorPoolInfo;
	DescriptorPoolInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolInfo.pNext			= nullptr;
	DescriptorPoolInfo.flags			= 0;
	DescriptorPoolInfo.maxSets			= ResourcesCount;
	DescriptorPoolInfo.poolSizeCount	= 1;
	DescriptorPoolInfo.pPoolSizes		= &DescriptorPoolSize;

	VkResult Result = vkCreateDescriptorPool(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorPoolInfo, nullptr, &_DescriptorPool);
	ETERNAL_ASSERT(!Result);
	// ALLOCATE DESCRIPTOR SETS
}
