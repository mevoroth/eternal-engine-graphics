#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

#include "Graphics/CommandList.hpp"
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
		class FrameBuffer;
		class VulkanRenderPass;
		class VulkanCommandAllocator;

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& DeviceObj, _In_ VulkanCommandAllocator& CommandAllocatorObj);
			~VulkanCommandList();

			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);

			void Begin(_In_ FrameBuffer& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanPipeline& Pipeline, _In_ VulkanRenderPass& RenderPassObj);
			void DrawPrimitive(_In_ uint32_t PrimitiveCount);
			void End();

			inline VkCommandBuffer_T*& GetVulkanCommandList() { return _CommandBuffer; }

		private:
			VulkanDevice& _Device;
			VulkanCommandAllocator& _CommandAllocator;
			VkCommandBuffer_T* _CommandBuffer = nullptr;
		};
	}
}

#endif
