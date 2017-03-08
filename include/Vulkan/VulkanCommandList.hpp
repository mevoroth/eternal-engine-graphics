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
<<<<<<< HEAD
<<<<<<< Updated upstream
=======
		class VulkanState;
		class VulkanPipeline;
		class VulkanFrameBuffer;
		class VulkanRenderPass;
>>>>>>> Stashed changes
=======
		class VulkanState;
		class VulkanPipeline;
		class VulkanFrameBuffer;
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce

		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanCommandQueue& CommandQueueObj);
			~VulkanCommandList();

<<<<<<< HEAD
<<<<<<< Updated upstream
			void Begin();
=======
			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline, _In_ VulkanRenderPass& RenderPassObj);
>>>>>>> Stashed changes
=======
			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline);
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
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
