#ifndef _D3D12_COMMAND_LIST_HPP_
#define _D3D12_COMMAND_LIST_HPP_

#include <cstdint>

struct ID3D12GraphicsCommandList;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;
		class D3D12CommandQueue;
		class D3D12State;
		class D3D12RenderTarget;
		class D3D12Constant;
		class Viewport;
		class BlendState;

		class D3D12CommandList
		{
		public:
			D3D12CommandList(D3D12Device& DeviceObj, D3D12CommandQueue& CommandQueue, D3D12State& State);

			void DrawPrimitive(_In_ uint32_t PrimitiveCount);

			void SetViewport(_In_ Viewport& ViewportObj);
			void SetScissorRectangle(_In_ Viewport& ViewportObj);
			
			void ClearRenderTarget(_In_ D3D12RenderTarget& RenderTargetObj);
			void BindRenderTarget(_In_ uint32_t Slot, _In_ D3D12RenderTarget& RenderTargetObj);
			void BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer);

		private:
			ID3D12GraphicsCommandList* _CommandList = nullptr;
		};
	}
}

#endif
