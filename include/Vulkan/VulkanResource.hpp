#ifndef _VULKAN_RESOURCE_HPP_
#define _VULKAN_RESOURCE_HPP_

struct VkBuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class VulkanDevice;
		class VulkanView;

		class VulkanResource
		{
			union VkResource
			{
				VkBuffer_T* Buffer;
				VkImage_T* Image;
			};
		public:
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
