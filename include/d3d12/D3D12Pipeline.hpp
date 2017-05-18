#ifndef _D3D12_PIPELINE_HPP_
#define _D3D12_PIPELINE_HPP_

#include <cstdint>
#include "Graphics/Pipeline.hpp"

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
		class RenderTarget;
		class RootSignature;
		class RenderPass;
		class Viewport;

		class D3D12Pipeline : public Pipeline
		{
		public:
			D3D12Pipeline(
				_In_ Device& DeviceObj,
				_In_ RootSignature& RootSignatureObj,
				_In_ InputLayout& InputLayoutObj,
				_In_ RenderPass& RenderPassObj,
				_In_ Shader& VS,
				_In_ Shader& PS,
				_In_ const DepthTest& DepthTestObj,
				_In_ const StencilTest& StencilTestObj,
				_In_ Viewport& ViewportObj
			);

			inline ID3D12PipelineState* GetD3D12PipelineState() { return _PipelineState; }
			RootSignature& GetRootSignature() { return _RootSignature; }

		private:
			RootSignature&			_RootSignature;
			ID3D12PipelineState*	_PipelineState = nullptr;
		};
	}
}

#endif
