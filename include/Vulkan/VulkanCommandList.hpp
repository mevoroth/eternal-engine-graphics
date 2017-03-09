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
		class VulkanSwapChain;
		class VulkanCommandQueue;
		class VulkanState;
		class VulkanPipeline;
		class VulkanFrameBuffer;
		class VulkanRenderPass;

		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj, _In_ VulkanCommandQueue& CommandQueueObj);
			~VulkanCommandList();

			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ VulkanFrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline, _In_ VulkanRenderPass& RenderPassObj);
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
