#include "Vulkan_deprecated/VulkanDescriptorHeap.hpp"

#include "Vulkan_deprecated/VulkanDevice.hpp"
#include "Vulkan_deprecated/VulkanRootSignature.hpp"
#include "Vulkan_deprecated/VulkanView.hpp"
#include "Vulkan_deprecated/VulkanDescriptorTable.h"

using namespace std;
using namespace Eternal::Graphics;

VulkanDescriptorHeap::VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount)
	: DescriptorHeap(Resources, ResourcesCount)
	, _Device(DeviceObj)
{
	//uint32_t PoolCount[ROOT_SIGNATURE_PARAMETER_COUNT];
	//memset(PoolCount, 0x0, sizeof(uint32_t) * ROOT_SIGNATURE_PARAMETER_COUNT);

	//vector<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;
	//DescriptorSetLayoutBindings.resize(ResourcesCount);
	//_DescriptorFormats.resize(ResourcesCount);

	//for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
	//{
	//	const RootSignatureParameter& Resource = Resources[ResourceIndex];
	//	++PoolCount[Resource.Parameter];

	//	DescriptorSetLayoutBindings[ResourceIndex].binding				= Resource.Register;
	//	DescriptorSetLayoutBindings[ResourceIndex].descriptorType		= VULKAN_DESCRIPTOR_TYPES[Resource.Parameter];
	//	DescriptorSetLayoutBindings[ResourceIndex].descriptorCount		= 1;
	//	DescriptorSetLayoutBindings[ResourceIndex].stageFlags			= RootSignatureAccessToVkShaderStageFlags(Resource.Access);
	//	DescriptorSetLayoutBindings[ResourceIndex].pImmutableSamplers	= nullptr;

	//	_DescriptorFormats[ResourceIndex]								= VULKAN_DESCRIPTOR_TYPES[Resource.Parameter];
	//}

	//vector<VkDescriptorPoolSize> DescriptorPoolsSize;
	//for (uint32_t DescriptorPoolIndex = 0; DescriptorPoolIndex < ROOT_SIGNATURE_PARAMETER_COUNT; ++DescriptorPoolIndex)
	//{
	//	uint32_t ResourceCount = PoolCount[DescriptorPoolIndex];
	//	if (ResourceCount)
	//	{
	//		VkDescriptorPoolSize NewPoolType;
	//		NewPoolType.type			= VULKAN_DESCRIPTOR_TYPES[DescriptorPoolIndex];
	//		NewPoolType.descriptorCount	= ResourceCount;
	//		DescriptorPoolsSize.push_back(NewPoolType);
	//	}
	//}
	//
	//VkDescriptorPoolCreateInfo DescriptorPoolInfo;
	//DescriptorPoolInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	//DescriptorPoolInfo.pNext			= nullptr;
	//DescriptorPoolInfo.flags			= 0;
	//DescriptorPoolInfo.maxSets			= 1; // MULTI BUFFERED???
	//DescriptorPoolInfo.poolSizeCount	= DescriptorPoolsSize.size();
	//DescriptorPoolInfo.pPoolSizes		= DescriptorPoolsSize.data();

	//VkResult Result = vkCreateDescriptorPool(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorPoolInfo, nullptr, &_DescriptorPool);
	//ETERNAL_ASSERT(!Result);

	//VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	//DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	//DescriptorSetLayoutInfo.pNext			= nullptr;
	//DescriptorSetLayoutInfo.flags			= 0;
	//DescriptorSetLayoutInfo.bindingCount	= DescriptorSetLayoutBindings.size();
	//DescriptorSetLayoutInfo.pBindings		= DescriptorSetLayoutBindings.data();

	//Result = vkCreateDescriptorSetLayout(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	//ETERNAL_ASSERT(!Result);

	//VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo;
	//DescriptorSetAllocateInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	//DescriptorSetAllocateInfo.pNext					= nullptr;
	//DescriptorSetAllocateInfo.descriptorPool		= _DescriptorPool;
	//DescriptorSetAllocateInfo.descriptorSetCount	= 1;
	//DescriptorSetAllocateInfo.pSetLayouts			= &_DescriptorSetLayout;

	//Result = vkAllocateDescriptorSets(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetAllocateInfo, &_DescriptorSet);
	//ETERNAL_ASSERT(!Result);
}

//VulkanDescriptorHeap::VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
//	: DescriptorHeap(Space, Register)
//	, _Device(DeviceObj)
//	//, _BoundResources(ResourcesCount)
//{
//	ETERNAL_BREAK(); // DEPRECATED
//	//VkDescriptorPoolSize DescriptorPoolSize;
//	//DescriptorPoolSize.type				= VULKAN_DESCRIPTOR_TYPES[HeapTypeObj];
//	//DescriptorPoolSize.descriptorCount	= ResourcesCount;
//
//	//VkDescriptorPoolCreateInfo DescriptorPoolInfo;
//	//DescriptorPoolInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//	//DescriptorPoolInfo.pNext			= nullptr;
//	//DescriptorPoolInfo.flags			= 0;
//	//DescriptorPoolInfo.maxSets			= ResourcesCount;
//	//DescriptorPoolInfo.poolSizeCount	= 1;
//	//DescriptorPoolInfo.pPoolSizes		= &DescriptorPoolSize;
//
//	//VkResult Result = vkCreateDescriptorPool(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorPoolInfo, nullptr, &_DescriptorPool);
//	//ETERNAL_ASSERT(!Result);
//
//	//VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding;
//	//DescriptorSetLayoutBinding.binding				= Space;
//	//DescriptorSetLayoutBinding.descriptorType		= VULKAN_DESCRIPTOR_TYPES[HeapTypeObj];
//	//DescriptorSetLayoutBinding.descriptorCount		= 1;
//	//DescriptorSetLayoutBinding.stageFlags			= RootSignatureAccessToVkShaderStageFlags(RootSignatureAccessObj);
//	//DescriptorSetLayoutBinding.pImmutableSamplers	= nullptr;
//
//	//VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
//	//DescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//	//DescriptorSetLayoutInfo.pNext			= nullptr;
//	//DescriptorSetLayoutInfo.flags			= 0;
//	//DescriptorSetLayoutInfo.bindingCount	= 1;
//	//DescriptorSetLayoutInfo.pBindings		= &DescriptorSetLayoutBinding;
//	//Result = vkCreateDescriptorSetLayout(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
//	//ETERNAL_ASSERT(!Result);
//
//	//_ResourcesPool.resize(ResourcesCount);
//
//	//vector<VkDescriptorSetLayout> DescriptorSetLayouts;
//	//DescriptorSetLayouts.resize(ResourcesCount);
//
//	//for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
//	//{
//	//	DescriptorSetLayouts[ResourceIndex] = _DescriptorSetLayout;
//	//}
//
//	//VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo;
//	//DescriptorSetAllocateInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//	//DescriptorSetAllocateInfo.pNext					= nullptr;
//	//DescriptorSetAllocateInfo.descriptorPool		= _DescriptorPool;
//	//DescriptorSetAllocateInfo.descriptorSetCount	= DescriptorSetLayouts.size();
//	//DescriptorSetAllocateInfo.pSetLayouts			= DescriptorSetLayouts.data();
//	//Result = vkAllocateDescriptorSets(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &DescriptorSetAllocateInfo, _ResourcesPool.data());
//	//ETERNAL_ASSERT(!Result);
//}

VulkanDescriptorHeap::~VulkanDescriptorHeap()
{
	//vkDestroyDescriptorPool(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _DescriptorPool, nullptr);
}

DescriptorTable* VulkanDescriptorHeap::CreateView(_In_ Device& DeviceObj)
{
	return new VulkanDescriptorTable(*this, GetResourcesCount());
}
