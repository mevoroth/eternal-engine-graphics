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
		class VulkanPipeline;
		class RenderTarget;
		class VulkanRenderPass;
		class CommandAllocator;
		class Pipeline;
		class RootSignature;
		class Resource;

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& DeviceObj, _In_ CommandAllocator& CommandAllocatorObj);
			~VulkanCommandList();

			virtual void SetViewport(_In_ Viewport& ViewportObj) override;
			virtual void SetScissorRectangle(_In_ Viewport& ViewportObj) override;
			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj) override;
			virtual void Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj) override;
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
