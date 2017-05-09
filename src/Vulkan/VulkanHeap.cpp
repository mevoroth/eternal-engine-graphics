#include "Vulkan/VulkanHeap.hpp"

#include <vulkan/vulkan.h>
#include <cstring>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include <algorithm>

using namespace Eternal::Graphics;

static VkMemoryPropertyFlagBits BuildMemoryPropertiesFlags(_In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
{
	return	(VkMemoryPropertyFlagBits)	((InVRAM ?			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT :	0)
									|	(VisibleFromCPU ?	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT	:	0)
									|	(Coherent ?			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT :	0)
									|	(Cached ?			VK_MEMORY_PROPERTY_HOST_CACHED_BIT :	0));
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

VulkanHeap::VulkanHeap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
	: Heap(DeviceObj, ResourcesCount)
{
	VkMemoryPropertyFlagBits MemoryPropertiesFlags = BuildMemoryPropertiesFlags(InVRAM, VisibleFromCPU, Coherent, Cached);
	ETERNAL_ASSERT(CheckMemoryPropertiesFlags(MemoryPropertiesFlags));
	_MemoryPropertiesFlags = MemoryPropertiesFlags;
}

void VulkanHeap::Initialize(_In_ size_t Size)
{
	ETERNAL_ASSERT(Size > 0);
	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(GetDevice());

	VkMemoryAllocateInfo MemoryAllocateInfo;
	MemoryAllocateInfo.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocateInfo.pNext			= nullptr;
	MemoryAllocateInfo.allocationSize	= Size * GetResourcesCount();
	MemoryAllocateInfo.memoryTypeIndex	= VulkanDeviceObj.FindBestMemoryTypeIndex(_MemoryPropertiesFlags);

	VkResult Result = vkAllocateMemory(VulkanDeviceObj.GetVulkanDevice(), &MemoryAllocateInfo, nullptr, &_DeviceMemory);
	ETERNAL_ASSERT(!Result);

	SetSize(Size);
}

VulkanHeap::~VulkanHeap()
{
	vkFreeMemory(static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice(), _DeviceMemory, nullptr);
}


