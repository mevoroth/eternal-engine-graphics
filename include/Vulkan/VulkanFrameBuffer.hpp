#ifndef _VULKAN_FRAME_BUFFER_HPP_
#define _VULKAN_FRAME_BUFFER_HPP_

#include <cstdint>
#include "Vulkan/VulkanResource.hpp"

struct VkFramebuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanFrameBuffer : public VulkanResource
		{
		public:
			VulkanFrameBuffer(_In_ VulkanDevice& DeviceObj, _In_ VkImage_T*& VulkanImage, _In_ VulkanView& View, _In_ uint32_t Width, _In_ uint32_t Height);

			VkFramebuffer_T*& GetFrameBuffer() { return _FrameBuffer; }

		private:
			VkFramebuffer_T* _FrameBuffer = nullptr;
		};
	}
}

#endif
