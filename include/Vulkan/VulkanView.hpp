<<<<<<< HEAD
#ifndef _VULKAN_VIEW_HPP_
#define _VULKAN_VIEW_HPP_

struct VkImage_T;
struct VkImageView_T;
struct VkBufferView_T;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class VulkanDevice;
		class VulkanResource;

		class VulkanView
		{
			union VkView
			{
				VkImageView_T* ImageView;
				VkBufferView_T* BufferView;
			};
		public:
			VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& View, _In_ const Format& FormatObj);
			VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VkImage_T*& BackBufferImage, _In_ const TextureView& View, _In_ const Format& FormatObj);
			virtual ~VulkanView();

			VkImageView_T*& GetImageView() { return _View.ImageView; }
			VkBufferView_T*& GetBufferView() { return _View.BufferView; }

			const Format& GetFormat() const { return _Format; }

		private:
			VkView _View;
			Format _Format;
		};
	}
}

#endif
=======
#ifndef _VULKAN_VIEW_HPP_
#define _VULKAN_VIEW_HPP_

struct VkImage_T;
struct VkImageView_T;
struct VkBufferView_T;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class VulkanDevice;
		class VulkanResource;

		class VulkanView
		{
			union VkView
			{
				VkImageView_T* ImageView;
				VkBufferView_T* BufferView;
			};
		public:
			VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& View, _In_ const Format& FormatObj);
			VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VkImage_T*& BackBufferImage, _In_ const TextureView& View, _In_ const Format& FormatObj);
			virtual ~VulkanView();

			VkImageView_T*& GetImageView() { return _View.ImageView; }
			VkBufferView_T*& GetBufferView() { return _View.BufferView; }

		private:
			VkView _View;
		};
	}
}

#endif
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce