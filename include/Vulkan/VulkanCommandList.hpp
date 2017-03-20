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
		class Device;
		class VulkanCommandQueue;
		class VulkanState;
		class RenderTarget;
		class VulkanRenderPass;
		class CommandAllocator;

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& DeviceObj, _In_ CommandAllocator& CommandAllocatorObj);
			~VulkanCommandList();

			virtual void SetViewport(_In_ Viewport& ViewportObj) override;
			virtual void SetScissorRectangle(_In_ Viewport& ViewportObj) override;

			void Begin(_In_ RenderTarget& FrameBufferObj, _In_ VulkanState& State, _In_ VulkanRenderPass& RenderPassObj);
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) override;
			virtual void End() override;

			inline VkCommandBuffer_T*& GetVulkanCommandList() { return _CommandBuffer; }

		private:
			Device&				_Device;
			CommandAllocator&	_CommandAllocator;
			VkCommandBuffer_T*	_CommandBuffer = nullptr;
		};
	}
}

#endif
