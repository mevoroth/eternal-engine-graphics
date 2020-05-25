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
				Device& InDevice,
				const PipelineCreateInformation& CreateInformation
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
