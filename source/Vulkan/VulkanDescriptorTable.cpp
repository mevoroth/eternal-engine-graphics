#include "Vulkan/VulkanDescriptorTable.h"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanDescriptorHeap.hpp"
#include "Vulkan/VulkanSampler.hpp"

using namespace Eternal::Graphics;

VulkanDescriptorTable::VulkanDescriptorTable(_In_ VulkanDescriptorHeap& DescriptorHeapObj, _In_ uint32_t ResourcesCount)
	: _DescriptorHeap(DescriptorHeapObj)
{
	ETERNAL_ASSERT(ResourcesCount > 0);
	_Resources.resize(ResourcesCount);
}

VkDescriptorSet_T*& VulkanDescriptorTable::GetDescriptorSet()
{
	return _DescriptorHeap.GetDescriptorSet();
}

void VulkanDescriptorTable::SetResource(_In_ uint32_t Slot, _In_ View* ViewObj)
{
	ETERNAL_ASSERT(Slot < _Resources.size());
	_Resources[Slot] = ViewObj;
}

void VulkanDescriptorTable::SetResource(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
{
	ETERNAL_ASSERT(Slot < _Resources.size());
	_Resources[Slot] = SamplerObj;
}

void VulkanDescriptorTable::Update(_In_ Device& DeviceObj)
{
	//vector<VkWriteDescriptorSet> WriteDescriptorSets;
	//WriteDescriptorSets.resize(_Resources.size());
	//
	//vector<VkDescriptorImageInfo> ImageInfos;
	//ImageInfos.resize(_Resources.size());

	//vector<VkDescriptorBufferInfo> BufferInfos;
	//BufferInfos.resize(_Resources.size());

	//vector<VkBufferView> BufferViews;
	//BufferViews.resize(_Resources.size());

	//uint32_t ImageInfoIndex = 0;
	//uint32_t BufferInfoIndex = 0;
	//uint32_t BufferViewIndex = 0;

	//for (uint32_t ResourceIndex = 0; ResourceIndex < _Resources.size(); ++ResourceIndex)
	//{
	//	const VkDescriptorType& CurrentDescriptorType = _DescriptorHeap.GetDescriptorTypes()[ResourceIndex];

	//	WriteDescriptorSets[ResourceIndex].sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	//	WriteDescriptorSets[ResourceIndex].pNext			= nullptr;
	//	WriteDescriptorSets[ResourceIndex].dstSet			= GetDescriptorSet();
	//	WriteDescriptorSets[ResourceIndex].dstBinding		= ResourceIndex;
	//	WriteDescriptorSets[ResourceIndex].dstArrayElement	= 0u;
	//	WriteDescriptorSets[ResourceIndex].descriptorCount	= 1u;
	//	WriteDescriptorSets[ResourceIndex].descriptorType	= CurrentDescriptorType;

	//	if (CurrentDescriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)
	//	{
	//		VkDescriptorImageInfo& ImageInfo = ImageInfos[ImageInfoIndex++];
	//		WriteDescriptorSets[ResourceIndex].pImageInfo	= &ImageInfo;
	//		ImageInfo.sampler								= reinterpret_cast<VulkanSampler*>(_Resources[ResourceIndex])->GetVulkanSampler();

	//		//VkSampler        sampler;
	//		//VkImageView      imageView;
	//		//VkImageLayout    imageLayout;
	//	}
	//	else if (CurrentDescriptorType == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
	//	{
	//		VkDescriptorImageInfo& ImageInfo = ImageInfos[ImageInfoIndex++];
	//		WriteDescriptorSets[ResourceIndex].pImageInfo	= &ImageInfo;
	//		ImageInfo.imageView								= reinterpret_cast<VulkanView*>(_Resources[ResourceIndex])->GetImageView();
	//	}
	//	else
	//	{
	//		ETERNAL_ASSERT(false);
	//	}
	//}
	//
	//vkUpdateDescriptorSets(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _Resources.size(), WriteDescriptorSets.data(), 0u, nullptr);
}
