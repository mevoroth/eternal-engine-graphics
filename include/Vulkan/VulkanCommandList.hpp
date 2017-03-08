#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

#include <cstdint>

struct VkCommandBuffer_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
		class VulkanCommandQueue;
<<<<<<< Updated upstream
=======
		class VulkanState;
		class VulkanPipeline;
		class VulkanFrameBuffer;
		class VulkanRenderPass;
>>>>>>> Stashed changes

		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanCommandQueue& CommandQueueObj);
			~VulkanCommandList();

<<<<<<< Updated upstream
			void Begin();
=======
			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline, _In_ VulkanRenderPass& RenderPassObj);
>>>>>>> Stashed changes
			void DrawPrimitive(_In_ uint32_t PrimitiveCount);
			void End();

		private:
			VulkanDevice& _Device;
			VulkanCommandQueue& _CommandQueue;
			VkCommandBuffer_T* _CommandBuffer = nullptr;
		};
	}
}

#endif
