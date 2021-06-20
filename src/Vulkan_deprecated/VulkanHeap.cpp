#include "Vulkan/VulkanHeap.hpp"

#include <cstring>
#include "Vulkan/VulkanDevice.hpp"
#include <algorithm>

using namespace Eternal::Graphics;

VulkanHeap::VulkanHeap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
	: Heap(DeviceObj, HeapTypeObj, ResourcesCount)
{
	//VkMemoryPropertyFlagBits MemoryPropertiesFlags = BuildMemoryPropertiesFlags(InVRAM, VisibleFromCPU, Coherent, Cached);
	//ETERNAL_ASSERT(CheckMemoryPropertiesFlags(MemoryPropertiesFlags));
	//_MemoryPropertiesFlags = MemoryPropertiesFlags;
}

void VulkanHeap::Initialize(_In_ uint32_t MemoryTypeBitsRequirement, _In_ size_t Size)
{
	_MemoryTypeBitsRequirement = MemoryTypeBitsRequirement;
	Initialize(Size);
}

void VulkanHeap::Initialize(_In_ size_t Size)
{
	//ETERNAL_ASSERT(!_DeviceMemory);
	//ETERNAL_ASSERT(Size > 0);
	//ETERNAL_ASSERT(_MemoryTypeBitsRequirement != ~0);
	//VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(GetDevice());

	//VkMemoryAllocateInfo MemoryAllocateInfo;
	//MemoryAllocateInfo.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	//MemoryAllocateInfo.pNext			= nullptr;
	//MemoryAllocateInfo.allocationSize	= Size * GetResourcesCount();
	//MemoryAllocateInfo.memoryTypeIndex	= VulkanDeviceObj.FindBestMemoryTypeIndex(_MemoryTypeBitsRequirement, _MemoryPropertiesFlags);

	//VkResult Result = vkAllocateMemory(VulkanDeviceObj.GetVulkanDevice(), &MemoryAllocateInfo, nullptr, &_DeviceMemory);
	//ETERNAL_ASSERT(!Result);

	SetSize(Size);
}

bool VulkanHeap::IsInitialized() const
{
	return _DeviceMemory != nullptr;
}

VulkanHeap::~VulkanHeap()
{
	//vkFreeMemory(static_cast<VulkanDevice&>(GetDevice()).GetVulkanDevice(), _DeviceMemory, nullptr);
}


