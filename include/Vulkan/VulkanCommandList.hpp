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

			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount) override;
			virtual void Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj) override;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) override;
			virtual void DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation) override;
			virtual void DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation) override;
			virtual void End() override;
			virtual void SetIndicesBuffer(_In_ Resource* IndicesBuffer) override;
			virtual void SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t VerticesBuffersCount, _In_ Resource* VerticesBuffers[]) override;
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj, RenderTarget& RenderTargetObj, _In_ Viewport& ViewportObj) override;
			virtual void EndRenderPass() override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) override;
			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) override;

			inline VkCommandBuffer_T*& GetVulkanCommandList() { return _CommandBuffer; }

		private:
			Device&				_Device;
			CommandAllocator&	_CommandAllocator;
			VkCommandBuffer_T*	_CommandBuffer = nullptr;
		};
	}
}

#endif
