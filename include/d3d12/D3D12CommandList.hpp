#pragma once

#include "Graphics/CommandList.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12Device.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Resource;

		using SetRoot32BitConstantsType = void (ID3D12GraphicsCommandList::*)(
			_In_ UINT													RootParameterIndex,
			_In_ UINT													Num32BitValuesToSet,
			_In_reads_(Num32BitValuesToSet * sizeof(UINT))	const void*	pSrcData,
			_In_ UINT													DestOffsetIn32BitValues
		);

		using SetRootShaderResourceViewType = void (ID3D12GraphicsCommandList::*)(
			_In_ UINT							RootParameterIndex,
			_In_ D3D12_GPU_VIRTUAL_ADDRESS		BufferLocation
		);

		using SetRootConstantBufferViewType = void (ID3D12GraphicsCommandList::*)(
			_In_ UINT							RootParameterIndex,
			_In_ D3D12_GPU_VIRTUAL_ADDRESS		BufferLocation
		);

		using SetRootUnorderedAccessViewType = void (ID3D12GraphicsCommandList::*)(
			_In_ UINT							RootParameterIndex,
			_In_ D3D12_GPU_VIRTUAL_ADDRESS		BufferLocation
			);

		using SetRootDescriptorTableType = void (ID3D12GraphicsCommandList::*)(
			_In_ UINT							RootParameterIndex,
			_In_ D3D12_GPU_DESCRIPTOR_HANDLE	BaseDescriptor
		);

		class D3D12CommandList final : public CommandList
		{
		public:
			D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~D3D12CommandList();

			virtual void SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName) override final;

			virtual void BeginEvent(_In_ GraphicsContext& InContext, _In_ const char* InEventName) override final;
			virtual void EndEvent(_In_ GraphicsContext& InContext) override final;

			virtual void Begin(_In_ GraphicsContext& InContext) override final;
			virtual void End() override final;
			
			virtual void BeginRenderPass(_In_ const RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override final;

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

			virtual void DispatchMesh(_In_ uint32_t InTaskBatchesCount = 1) override final;

			virtual void SetRayTracingPipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void DispatchRays(_In_ const ShaderTable& InShaderTable, _In_ uint32_t InX /* = 1 */, _In_ uint32_t InY /* = 1 */) override final;
			virtual void BuildRaytracingAccelerationStructure(_In_ GraphicsContext& InContext, _In_ AccelerationStructure& InAccelerationStructure) override final;

			virtual void CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion) override final;

			inline ID3D12CommandList* GetD3D12CommandList() { return _GraphicCommandList6; }
			inline D3D12CommandAllocator& GetD3D12CommandAllocator() { return static_cast<D3D12CommandAllocator&>(GetCommandAllocator()); }
			inline D3D12Device& GetD3D12Device() { return static_cast<D3D12Device&>(GetDevice()); }

		private:
			void _CopyResourceToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			void _CopyResourceToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			template<
				SetRoot32BitConstantsType SetRoot32BitConstants,
				SetRootShaderResourceViewType SetRootShaderResourceView,
				SetRootConstantBufferViewType SetRootConstantBufferView,
				SetRootUnorderedAccessViewType SetRootUnorderedAccessView,
				SetRootDescriptorTableType SetRootDescriptorTable
			>
			void _SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable);

			ID3D12GraphicsCommandList6*	_GraphicCommandList6	= nullptr;
		};
	}
}
