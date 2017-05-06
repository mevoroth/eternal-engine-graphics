#ifndef _VULKAN_VIEW_HPP_
#define _VULKAN_VIEW_HPP_

#include <cstdint>
#include "Graphics/View.hpp"

struct VkImage_T;
struct VkImageView_T;
struct VkBufferView_T;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class Device;
		class VulkanResource;

		class VulkanView : public View
		{
			union VkView
			{
				VkImageView_T* ImageView;
				VkBufferView_T* BufferView;
			};
		public:
			VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			VulkanView(_In_ Device& DeviceObj, _In_ VkImage_T*& BackBufferImage, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull);
			virtual ~VulkanView();

			virtual RenderTarget& GetAsRenderTarget() override;

			VkImageView_T*& GetImageView() { return _View.ImageView; }
			VkBufferView_T*& GetBufferView() { return _View.BufferView; }

			const Format& GetFormat() const { return _Format; }

		private:
			VkView			_View;
			Format			_Format;
		};
	}
}

#endif
