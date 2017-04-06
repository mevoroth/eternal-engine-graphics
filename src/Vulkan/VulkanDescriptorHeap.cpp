#include "Vulkan/VulkanDescriptorHeap.hpp"

#include <vector>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRootSignature.hpp"

using namespace std;
using namespace Eternal::Graphics;

static const VkShaderStageFlags RootSignatureAccessToVkShaderStageFlags(_In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	return	(RootSignatureAccessObj & ROOT_SIGNATURE_VS) ? VK_SHADER_STAGE_VERTEX_BIT					: 0
		|	(RootSignatureAccessObj & ROOT_SIGNATURE_HS) ? VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT		: 0
		|	(RootSignatureAccessObj & ROOT_SIGNATURE_DS) ? VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT	: 0
		|	(RootSignatureAccessObj & ROOT_SIGNATURE_GS) ? VK_SHADER_STAGE_GEOMETRY_BIT					: 0
		|	(RootSignatureAccessObj & ROOT_SIGNATURE_PS) ? VK_SHADER_STAGE_COMPUTE_BIT					: 0;
}

VulkanDescriptorHeap::VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
	: _Device(DeviceObj)
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

	VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding;
	DescriptorSetLayoutBinding.binding				= 0;
	DescriptorSetLayoutBinding.descriptorType		= VULKAN_DESCRIPTOR_TYPES[HeapTypeObj];
	DescriptorSetLayoutBinding.descriptorCount		= 1;
	DescriptorSetLayoutBinding.stageFlags			= RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
	DescriptorSetLayoutBinding.pImmutableSamplers	= nullptr;

	VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorSetLayoutInfo.pNext			= nullptr;
	DescriptorSetLayoutInfo.flags			= 0;
	DescriptorSetLayoutInfo.bindingCount	= 1;
	DescriptorSetLayoutInfo.pBindings		= &DescriptorSetLayoutBinding;
	Result = vkCreateDescriptorSetLayout(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	ETERNAL_ASSERT(!Result);

	_ResourcesPool.resize(ResourcesCount);

	vector<VkDescriptorSetLayout> DescriptorSetLayouts;
	DescriptorSetLayouts.resize(ResourcesCount);

	for (unsigned int ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
	{
		DescriptorSetLayouts[ResourceIndex] = _DescriptorSetLayout;
	}

	VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo;
	DescriptorSetAllocateInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	DescriptorSetAllocateInfo.pNext					= nullptr;
	DescriptorSetAllocateInfo.descriptorPool		= _DescriptorPool;
	DescriptorSetAllocateInfo.descriptorSetCount	= DescriptorSetLayouts.size();
	DescriptorSetAllocateInfo.pSetLayouts			= DescriptorSetLayouts.data();
	Result = vkAllocateDescriptorSets(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetAllocateInfo, _ResourcesPool.data());
	ETERNAL_ASSERT(!Result);
}

VulkanDescriptorHeap::~VulkanDescriptorHeap()
{
	vkDestroyDescriptorPool(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _DescriptorPool, nullptr);
}

VkDescriptorSet_T* VulkanDescriptorHeap::Pop()
{
	VkDescriptorSet DescriptorSet = _ResourcesPool.back();
	_ResourcesPool.pop_back();
	return DescriptorSet;
}

void VulkanDescriptorHeap::Push(_In_ VkDescriptorSet_T* Handle)
{
	_ResourcesPool.push_back(Handle);
}
