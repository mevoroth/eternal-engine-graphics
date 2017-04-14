#ifndef _D3D12_COMMAND_LIST_HPP_
#define _D3D12_COMMAND_LIST_HPP_

#include "Graphics/CommandList.hpp"

#include <cstdint>

struct ID3D12GraphicsCommandList;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;
		class CommandAllocator;
		class View;
		class D3D12Pipeline;
		class D3D12RenderTarget;
		class D3D12Constant;
		class Viewport;
		class BlendState;
		class Pipeline;
		class RootSignature;

		class D3D12CommandList : public CommandList
		{
		public:
			D3D12CommandList(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueue, _In_ D3D12Pipeline& State);

			virtual void SetViewport(_In_ Viewport& ViewportObj) override;
			virtual void SetScissorRectangle(_In_ Viewport& ViewportObj) override;
			
			void ClearRenderTarget(_In_ View& RenderTargetView);
			void BindRenderTarget(_In_ uint32_t Slot, _In_ View& RenderTargetView);
			void BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer);

			virtual void BindPipelineInput(_In_ RootSignature& RootSignatureObj) override;
			virtual void Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj) override;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) override;
			virtual void End() override;
			virtual void BeginRenderPass(_In_ RenderPass& RenderPassObj, RenderTarget& RenderTargetObj, _In_ Viewport& ViewportObj) override;
			virtual void EndRenderPass() override;
			virtual void CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination) override;

			virtual void Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount) override;

			ID3D12GraphicsCommandList*& GetD3D12GraphicsCommandList() { return _CommandList; }

		private:
			void Copy(_In_ Resource& Source, _In_ Resource& Destination);

			ID3D12GraphicsCommandList* _CommandList = nullptr;
		};
	}
}

#endif
