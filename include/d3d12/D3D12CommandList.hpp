#ifndef _D3D12_COMMAND_LIST_HPP_
#define _D3D12_COMMAND_LIST_HPP_

#include "Graphics/CommandList.hpp"

#include <cstdint>
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;
		class CommandAllocator;
		class View;
		class Pipeline;
		class D3D12RenderTarget;
		class D3D12Constant;
		class Viewport;
		class BlendState;
		class RootSignature;

		extern const D3D12_COMMAND_LIST_TYPE D3D12_COMMAND_LIST_TYPES[];

		class D3D12CommandList : public CommandList
		{
		public:
			D3D12CommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type, _In_ Pipeline* DefaultPipeline = nullptr);
			virtual ~D3D12CommandList();

			virtual void SetViewport(_In_ const Viewport& ViewportObj) override;
			virtual void SetScissorRectangle(_In_ const Viewport& ViewportObj) override;
			
			//void ClearRenderTarget(_In_ View& RenderTargetView);
			//void BindRenderTarget(_In_ uint32_t Slot, _In_ View& RenderTargetView);
			//void BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer);

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
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj) override;
			virtual void EndRenderPass() override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination, uint64_t SourceOffset, uint64_t DestinationOffset, uint64_t Size) override;
			virtual void CopyTexture(_In_ Resource& Source, _In_ Resource& Destination, _In_ const Position3D& SourcePosition, _In_ const Position3D& DestinationPosition, _In_ const Extent& Size) override;
			virtual void CopyBufferToTexture(_In_ Resource& Buffer, _In_ Resource& Texture, uint64_t BufferOffset, uint64_t BufferSize, _In_ const Position3D& TexturePosition, _In_ const Extent& Size) override;
			virtual void SetIndicesBuffer(_In_ Resource* IndicesBuffer) override;
			virtual void SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t NumViews, _In_ Resource* VerticesBuffers[]) override;

			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) override;

			ID3D12GraphicsCommandList*& GetD3D12GraphicsCommandList() { return _CommandList; }

		private:
			void _Copy(_In_ Resource& Source, _In_ Resource& Destination);

			ID3D12GraphicsCommandList* _CommandList = nullptr;
		};
	}
}

#endif
