#ifndef _VULKAN_VIEW_HPP_
#define _VULKAN_VIEW_HPP_

#include <vulkan/vulkan.hpp>
#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum TextureView;
		class Device;
		class VulkanResource;

		class VulkanView : public View
		{
			union VkView
			{
				vk::ImageView	ImageView;
				vk::BufferView	BufferView;
			};
		public:
			VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			VulkanView(_In_ Device& DeviceObj, _In_ const vk::Image& BackBufferImage, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull);
			virtual ~VulkanView();

			//virtual RenderTarget& GetAsRenderTarget() override;

			vk::ImageView&	GetImageView()		{ return _ImageView; }
			vk::BufferView&	GetBufferView()		{ return _BufferView; }

		private:
			//VkView _View;
			vk::ImageView _ImageView;
			vk::BufferView _BufferView;
		};
	}
}

#endif
