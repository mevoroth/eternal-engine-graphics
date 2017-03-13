#ifndef _D3D12_STATE_HPP_
#define _D3D12_STATE_HPP_

#include <cstdint>

struct ID3D12PipelineState;
struct ID3D12RootSignature;

namespace Eternal
{
	namespace Graphics
	{
		class InputLayout;
		class Device;
		class Shader;
		class DepthTest;
		class StencilTest;
		class BlendState;
		class RenderTarget;
		class D3D12Sampler;

		class D3D12State
		{
		public:
			D3D12State(
				_In_ Device& DeviceObj,
				_In_ InputLayout& InputLayoutObj,
				_In_ Shader* VS,
				_In_ Shader* PS,
				_In_ const DepthTest& DepthTestObj,
				_In_ const StencilTest& StencilTestObj,
				_In_ const BlendState BlendStates[],
				//_In_ const FrameBuffer* RenderTargets[],
				_In_ uint32_t RenderTargetsCount,
				_In_ const D3D12Sampler Samplers[],
				_In_ uint32_t SamplersCount
			);

			inline ID3D12PipelineState* GetD3D12PipelineState() { return _PipelineState; }
			inline ID3D12RootSignature* GetD3D12RootSignature() { return _RootSignature; }

		private:
			ID3D12RootSignature* _RootSignature = nullptr;
			ID3D12PipelineState* _PipelineState = nullptr;
		};
	}
}

#endif
