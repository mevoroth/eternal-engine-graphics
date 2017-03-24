#include "Vulkan/VulkanHeap.hpp"

#include <vulkan/vulkan.h>
#include <cstring>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include <algorithm>

using namespace Eternal::Graphics;

static VkMemoryPropertyFlagBits BuildMemoryPropertiesFlags(_In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
{
	return	(VkMemoryPropertyFlagBits)	(InVRAM ?			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT :	0
									|	VisibleFromCPU ?	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT	:	0
									|	Coherent ?			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT :	0
									|	Cached ?			VK_MEMORY_PROPERTY_HOST_CACHED_BIT :	0);
}

static bool CheckMemoryPropertiesFlags(_In_ const VkMemoryPropertyFlagBits& Flags)
{
	VkMemoryPropertyFlagBits AllowedMemoryProperties[] =
	{
		(VkMemoryPropertyFlagBits)0,
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
	};
	VkMemoryPropertyFlagBits* EndAllowedMemoryProperties = AllowedMemoryProperties + ETERNAL_ARRAYSIZE(AllowedMemoryProperties);
	return std::find(AllowedMemoryProperties, EndAllowedMemoryProperties, Flags) != EndAllowedMemoryProperties;
}

VulkanHeap::VulkanHeap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
	: _Device(DeviceObj)
{
	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj);

	VkMemoryPropertyFlagBits MemoryPropertiesFlags = BuildMemoryPropertiesFlags(InVRAM, VisibleFromCPU, Coherent, Cached);
	ETERNAL_ASSERT(CheckMemoryPropertiesFlags(MemoryPropertiesFlags));

	VkMemoryAllocateInfo MemoryAllocateInfo;
	MemoryAllocateInfo.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.pNext			= nullptr;
	MemoryAllocateInfo.allocationSize	= Size;
	MemoryAllocateInfo.memoryTypeIndex	= VulkanDeviceObj.FindBestMemoryTypeIndex(MemoryPropertiesFlags);

	VkResult Result = vkAllocateMemory(VulkanDeviceObj.GetVulkanDevice(), &MemoryAllocateInfo, nullptr, &_DeviceMemory);
	ETERNAL_ASSERT(!Result);
}

VulkanHeap::~VulkanHeap()
{
	vkFreeMemory(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _DeviceMemory, nullptr);
}


