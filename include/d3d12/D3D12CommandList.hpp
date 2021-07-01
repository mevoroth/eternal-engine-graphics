#pragma once

#include "Graphics/CommandList.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12Device.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class Resource;
		class RootSignature;

		class D3D12CommandList final : public CommandList
		{
		public:
			D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~D3D12CommandList();

			virtual void Begin(_In_ GraphicsContext& InContext) override final;
			virtual void End() override final;
			
			virtual void BeginRenderPass(_In_ const RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override final;

			virtual void SetGraphicsPipeline(_In_ const Pipeline& InPipeline) override final;
			virtual void SetGraphicsDescriptorTable(_In_ DescriptorTable& InDescriptorTable) override final;
			virtual void DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;
			virtual void DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount = 1, _In_ uint32_t InFirstIndex = 0, _In_ uint32_t InFirstVertex = 0, _In_ uint32_t InFirstInstance = 0) override final;

			virtual void CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion) override final;

			inline ID3D12CommandList* GetD3D12CommandList() { return _GraphicCommandList5; }
			inline D3D12CommandAllocator& GetD3D12CommandAllocator() { return static_cast<D3D12CommandAllocator&>(GetCommandAllocator()); }
			inline D3D12Device& GetD3D12Device() { return static_cast<D3D12Device&>(GetDevice()); }

		//private:
			void _CopyResourceToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);
			void _CopyResourceToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion);

			ID3D12GraphicsCommandList5*	_GraphicCommandList5	= nullptr;
			const RootSignature*		_CurrentRootSignature	= nullptr;
		};
	}
}
