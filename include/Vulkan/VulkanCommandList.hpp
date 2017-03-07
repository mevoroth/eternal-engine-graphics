#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

#include <cstdint>

struct VkCommandBuffer_T;

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;
		class VulkanDevice;
		class VulkanCommandQueue;
		class VulkanState;
		class VulkanPipeline;
		class VulkanFrameBuffer;

		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& Device, _In_ VulkanCommandQueue& CommandQueue);
			~VulkanCommandList();

			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline);
			void DrawPrimitive(_In_ uint32_t PrimitiveCount);
			void End();

			inline VkCommandBuffer_T*& GetVulkanCommandList() { return _CommandBuffer; }

		private:
			VulkanDevice& _Device;
			VulkanCommandQueue& _CommandQueue;
			VkCommandBuffer_T* _CommandBuffer = nullptr;
		};
	}
}

#endif
