#include "Vulkan/VulkanResource.hpp"

#include "Macros/Macros.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VK_BUFFER_USAGE_TRANSFER_SRC_BIT = 0x00000001,
VK_BUFFER_USAGE_TRANSFER_DST_BIT = 0x00000002,
VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT = 0x00000010,
VK_BUFFER_USAGE_STORAGE_BUFFER_BIT = 0x00000020,

static VkBufferUsageFlagBits RESOURCE_TYPE_TO_USAGE[] =
{
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
};

static VkBufferUsageFlagBits BuildUsage(_In_ const ResourceType& Type, _In_ bool Writable)
{
	switch (Type)
	{
	case INDEX:
		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case VERTEX:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case INDIRECT:
		return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
	}

	return RESOURCE_TYPE_TO_USAGE[Type] << (Writable ? 1 : 0);
}

VulkanResource::VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const ResourceType& Type, _In_ bool Writable)
{
	VkBufferCreateInfo BufferInfo;
	BufferInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferInfo.pNext					= nullptr;
	BufferInfo.flags					= 0;
	BufferInfo.size						= Size;
	BufferInfo.usage					= BuildUsage(Type, Writable);
	BufferInfo.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
	BufferInfo.queueFamilyIndexCount	= 0;		// Ignored because of VK_SHARING_MODE_EXCLUSIVE
	BufferInfo.pQueueFamilyIndices		= nullptr;	// Ignored because of VK_SHARING_MODE_EXCLUSIVE

	VkResult Result = vkCreateBuffer(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &BufferInfo, nullptr, &_Resource.Buffer);
	ETERNAL_ASSERT(!Result);
}

VulkanResource::~VulkanResource()
{
	ETERNAL_ASSERT(!_Resource.Buffer);
}

VulkanView* VulkanResource::CreateView(_In_ VulkanDevice& DeviceObj, _In_ const TextureView& View, _In_ const Format& FormatObj)
{
	VulkanView* VkView = new VulkanView(DeviceObj, *this, View, FormatObj);
	return VkView;
}
