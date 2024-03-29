#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class VulkanCommandAllocator;

		class VulkanCommandList final : public CommandList
		{
		public:
			static constexpr vk::ShaderStageFlagBits InvalidShaderStages = vk::ShaderStageFlagBits(~0);
			static constexpr uint32_t MaxDescriptorSetsCount = 128;

			VulkanCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~VulkanCommandList();

			virtual void SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName) override final;

			virtual void BeginEvent(_In_ GraphicsContext& InContext, _In_ const char* InEventName) override final;
			virtual void EndEvent(_In_ GraphicsContext& InContext) override final;

			virtual void Begin(_In_ GraphicsContext& InContext) override final;
			virtual void End() override final;

			virtual void BeginRenderPass(const RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override final;
			virtual void TransitionUAV(_In_ Resource* InResources[], _In_ uint32_t InResourcesCount) override final;

			virtual void SetViewport(_In_ const Viewport& InViewport) override final;
			virtual void SetScissorRectangle(_In_ const ScissorRectangle& InScissorRectangle) override final;
			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset = 0, _In_ const IndexBufferType& InIndexBufferType = IndexBufferType::INDEX_BUFFER_TYPE_16BITS) override final;
			virtual void SetVertexBuffers(_In_ const Resource* InVertexBuffers[], _In_ uint32_t InBufferCount = 1, _In_ uint32_t InFirstVertexBuffer = 0, _In_ VertexBufferParameters InParameters[] = {}) override final;
			virtual void SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable) override final;
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;

			virtual void SetComputePipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void SetComputeDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable) override final;
			virtual void Dispatch(_In_ uint32_t InX = 1, _In_ uint32_t InY = 1, _In_ uint32_t InZ = 1) override final;

			virtual void DispatchMesh(_In_ uint32_t InTaskBatchesCount /* = 1 */) override final;

			virtual void SetRayTracingPipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void DispatchRays(_In_ const ShaderTable& InShaderTable, _In_ uint32_t InX /* = 1 */, _In_ uint32_t InY /* = 1 */) override final;
			virtual void BuildRaytracingAccelerationStructure(_In_ GraphicsContext& InContext, _In_ AccelerationStructure& InAccelerationStructure) override final;

			virtual void CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion) override final;

			inline vk::CommandBuffer& GetVulkanCommandBuffer() { return _CommandBuffer; }
			inline VulkanCommandAllocator& GetVulkanCommandAllocator() { return static_cast<VulkanCommandAllocator&>(GetCommandAllocator()); }
			inline VulkanDevice& GetVulkanDevice() { return static_cast<VulkanDevice&>(GetDevice()); }

		private:
			template<vk::PipelineBindPoint VulkanPipelineBindPoint>
			void _SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);
			void _CopyBufferToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			void _CopyTextureToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			void _CopyBufferToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			void _CopyTextureToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			vk::CommandBuffer		_CommandBuffer;
			vk::ShaderStageFlagBits	_CurrentShaderStages = InvalidShaderStages;
		};
	}
}

#endif
