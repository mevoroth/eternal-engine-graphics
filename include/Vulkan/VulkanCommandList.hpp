#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

#include "Graphics/CommandList.hpp"
#include <cstdint>
#include <vector>

struct VkCommandBuffer_T;
struct VkPipelineLayout_T;
struct VkDescriptorSet_T;
enum VkImageLayout;

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
		enum CommandListType;

		VkImageLayout BuildImageLayout(const TransitionState& State);

		class VulkanCommandList : public CommandList
		{
		public:
			VulkanCommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type);
			~VulkanCommandList();

			virtual void SetViewport(_In_ const Viewport& ViewportObj) override;
			virtual void SetScissorRectangle(_In_ const Viewport& ViewportObj) override;

			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount) override;
			virtual void BindConstantBuffer(_In_ uint32_t Slot, _In_ View& ConstantBuffer) override;
			virtual void BindDescriptorTable(_In_ uint32_t Slot, _In_ View& DescriptorTable) override;
			virtual void BindBuffer(_In_ uint32_t Slot, _In_ View& Buffer) override;
			virtual void BindUAV(_In_ uint32_t Slot, _In_ View& UAV) override;
			virtual void Begin() override;
			virtual void Begin(_In_ Pipeline& PipelineObj) override;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) override;
			virtual void DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation) override;
			virtual void DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation) override;
			virtual void End() override;
			virtual void SetIndicesBuffer(_In_ Resource* IndicesBuffer) override;
			virtual void SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t VerticesBuffersCount, _In_ Resource* VerticesBuffers[]) override;
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj) override;
			virtual void EndRenderPass() override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination, uint64_t SourceOffset, uint64_t DestinationOffset, uint64_t Size) override;
			virtual void CopyTexture(_In_ Resource& Source, _In_ Resource& Destination, _In_ const Position3D& SourcePosition, _In_ const Position3D& DestinationPosition, _In_ const Extent& Size) override;
			virtual void CopyBufferToTexture(_In_ Resource& Buffer, _In_ Resource& Texture, uint64_t BufferOffset, uint64_t BufferSize, _In_ const Position3D& TexturePosition, _In_ const Extent& Size) override;
			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) override;

			inline VkCommandBuffer_T*& GetVulkanCommandList() { return _CommandBuffer; }

		private:
			void _Commit();

			struct CommandListCache
			{
				VkDescriptorSet_T*	DescriptorTables[MAX_BINDABLE_RESOURCES];
				uint32_t			DescriptorTablesCount	= 0u;
				VkPipelineLayout_T*	PipelineLayout			= nullptr;

				VulkanRenderPass*	RenderPass				= nullptr;

				bool				Dirty					= false;
			};

			CommandListCache	_CommandListCache;
			Device&				_Device;
			VkCommandBuffer_T*	_CommandBuffer = nullptr;
		};
	}
}

#endif
