#include "Vulkan/VulkanResource.hpp"

#include "Macros/Macros.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanHeap.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Vulkan/VulkanCommandList.hpp"

using namespace Eternal::Graphics;

static const VkBufferUsageFlagBits RESOURCE_TYPE_TO_USAGE[] =
{
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
};

static VkBufferUsageFlagBits BuildUsage(_In_ const BufferType& Type, _In_ bool Writable)
{
	switch (Type)
	{
	case BUFFER_INDEX:
		return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	case BUFFER_VERTEX:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case BUFFER_INDIRECT:
		return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
	}

	return (VkBufferUsageFlagBits)(RESOURCE_TYPE_TO_USAGE[Type] << (Writable ? 1 : 0));
}

VulkanResource::VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type/*, _In_ bool Writable*/)
	: Resource(HeapObj, Stride, Size)
{
	ETERNAL_ASSERT(Size > 0ull);

	VkBufferCreateInfo BufferInfo;
	BufferInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferInfo.pNext					= nullptr;
	BufferInfo.flags					= 0;
	BufferInfo.size						= Size;
	BufferInfo.usage					= (VkBufferUsageFlagBits)Type;//BuildUsage(Type, Writable);
	BufferInfo.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
	BufferInfo.queueFamilyIndexCount	= 0;		// Ignored because of VK_SHARING_MODE_EXCLUSIVE
	BufferInfo.pQueueFamilyIndices		= nullptr;	// Ignored because of VK_SHARING_MODE_EXCLUSIVE

	VkResult Result = vkCreateBuffer(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &BufferInfo, nullptr, &_Resource.Buffer);
	ETERNAL_ASSERT(!Result);
	
	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _Resource.Buffer, &MemoryRequirements);

	if (!HeapObj.IsInitialized())
		HeapObj.Initialize(MemoryRequirements.size);
	SetHeapSlot(HeapObj.Pop());

	Result = vkBindBufferMemory(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _Resource.Buffer, static_cast<VulkanHeap&>(HeapObj).GetVulkanDeviceMemory(), GetHeapSlot());
	ETERNAL_ASSERT(!Result);
}

VulkanResource::VulkanResource(_In_ VkImage_T* Image)
	: Resource()
{
	_Resource.Image = Image;
}

VulkanResource::VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& InitialState)
	: Resource(HeapObj, FormatObj, Width, Height, Depth, MipCount)
{
	ETERNAL_ASSERT(Width > 0);
	ETERNAL_ASSERT(Height > 0);
	ETERNAL_ASSERT(Depth > 0);
#ifdef ETERNAL_DEBUG
	if (Dimension == RESOURCE_DIMENSION_TEXTURE_1D)
	{
		ETERNAL_ASSERT(Height == 1);
	}
#endif

	VkImageCreateInfo ImageInfo;
	ImageInfo.sType					= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ImageInfo.pNext					= nullptr;
	ImageInfo.flags					= 0;
	ImageInfo.imageType				= (VkImageType)Dimension;
	ImageInfo.format				= VULKAN_FORMATS[FormatObj];
	ImageInfo.extent.width			= Width;
	ImageInfo.extent.height			= Height;
	ImageInfo.extent.depth			= (Dimension == RESOURCE_DIMENSION_TEXTURE_3D) ? Depth : 1u;
	ImageInfo.mipLevels				= MipCount;
	ImageInfo.arrayLayers			= (Dimension != RESOURCE_DIMENSION_TEXTURE_3D) ? Depth : 1u;
	ImageInfo.samples				= VK_SAMPLE_COUNT_1_BIT;
	ImageInfo.tiling				= VK_IMAGE_TILING_OPTIMAL;
	ImageInfo.usage					= Type;
	ImageInfo.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	ImageInfo.queueFamilyIndexCount = 0;		// Ignored because of VK_SHARING_MODE_EXCLUSIVE
	ImageInfo.pQueueFamilyIndices	= nullptr;	// Ignored because of VK_SHARING_MODE_EXCLUSIVE
	ImageInfo.initialLayout			= VK_IMAGE_LAYOUT_PREINITIALIZED;//BuildImageLayout(InitialState);

	VkResult Result = vkCreateImage(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &ImageInfo, nullptr, &_Resource.Image);
	ETERNAL_ASSERT(!Result);

	VkMemoryRequirements MemoryRequirements;
	vkGetImageMemoryRequirements(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _Resource.Image, &MemoryRequirements);
	
	if (!HeapObj.IsInitialized())
		HeapObj.Initialize(MemoryRequirements.size);
	SetHeapSlot(HeapObj.Pop());

	Result = vkBindImageMemory(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _Resource.Image, static_cast<VulkanHeap&>(HeapObj).GetVulkanDeviceMemory(), GetHeapSlot());
	ETERNAL_ASSERT(!Result);
}

VulkanResource::~VulkanResource()
{
	ETERNAL_ASSERT(!_Resource.Buffer);
}

View* VulkanResource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
{
	VulkanView* VkView = new VulkanView(DeviceObj, *this, ViewType, FormatObj);
	return VkView;
}

View* VulkanResource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset, _In_ uint64_t Size)
{
	VulkanView* VkView = new VulkanView(DeviceObj, *this, FormatObj, Offset, Size);
	return VkView;
}

View* VulkanResource::CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj)
{
	return CreateView(DeviceObj, DescriptorHeapObj, TEXTURE_VIEW_TYPE_2D, FormatObj);
}

View* VulkanResource::CreateDepthStencilView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj)
{
	ETERNAL_ASSERT(FormatObj == FORMAT_D32);
	return CreateView(DeviceObj, DescriptorHeapObj, TEXTURE_VIEW_TYPE_2D, FormatObj);
}

VulkanHeap& VulkanResource::GetVulkanHeap()
{
	return static_cast<VulkanHeap&>(GetHeap());
}

void* VulkanResource::Map(_In_ Device& DeviceObj)
{
	void* Data = nullptr;
	VkResult Result = vkMapMemory(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), GetVulkanHeap().GetVulkanDeviceMemory(), 0, GetBufferSize(), 0, &Data);
	ETERNAL_ASSERT(!Result);
	ETERNAL_ASSERT(Data);
	return Data;
}

void VulkanResource::Unmap(_In_ Device& DeviceObj)
{
	vkUnmapMemory(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), GetVulkanHeap().GetVulkanDeviceMemory());
}

void VulkanResource::SetName(_In_ const wchar_t* Name)
{
	ETERNAL_ASSERT(false);
}
