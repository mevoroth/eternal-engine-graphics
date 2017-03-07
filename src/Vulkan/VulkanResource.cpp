#include "Vulkan/VulkanResource.hpp"

#include "Macros/Macros.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanResource::~VulkanResource()
{
	ETERNAL_ASSERT(!_Resource.Buffer);
}

VulkanView* VulkanResource::CreateView(_In_ VulkanDevice& DeviceObj, _In_ const TextureView& View, _In_ const Format& FormatObj)
{
	VulkanView* VkView = new VulkanView(DeviceObj, *this, View, FormatObj);
	return VkView;
}
