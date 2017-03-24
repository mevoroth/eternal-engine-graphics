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

		class VulkanResource : Resource
		{
			union VkResource
			{
				VkBuffer_T* Buffer;
				VkImage_T* Image;
			};
		public:
			VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const ResourceType& Type, _In_ bool Writable = false);
			virtual ~VulkanResource();

			VkBuffer_T*& GetBuffer() { return _Resource.Buffer; }
			VkImage_T*& GetImage() { return _Resource.Image; }

			VulkanView* CreateView(_In_ VulkanDevice& DeviceObj, _In_ const TextureView& View, _In_ const Format& FormatObj);

		private:
			VkResource _Resource;
		};
	}
}

#endif
