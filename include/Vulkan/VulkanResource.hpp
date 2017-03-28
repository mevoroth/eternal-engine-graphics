#ifndef _VULKAN_RESOURCE_HPP_
#define _VULKAN_RESOURCE_HPP_

#include <cstdint>
#include "NextGenGraphics/Resource.hpp"

struct VkBuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class Device;
		class Heap;
		class VulkanDevice;
		class VulkanView;
		class VulkanHeap;

		class VulkanResource : public Resource
		{
			union VkResource
			{
				VkBuffer_T* Buffer;
				VkImage_T* Image;
			};
		public:
			VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const ResourceType& Type, _In_ bool Writable = false);
			VulkanResource(_In_ VkImage_T* Image);
			virtual ~VulkanResource();

			VkBuffer_T*& GetBuffer() { return _Resource.Buffer; }
			VkImage_T*& GetImage() { return _Resource.Image; }

			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) override;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj) override;

		private:
			VkResource	_Resource;
		};
	}
}

#endif
