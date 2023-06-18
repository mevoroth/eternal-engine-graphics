#pragma once

#include "Graphics/Pipeline.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class D3D12RootSignature;

		class D3D12Pipeline final : public Pipeline
		{
		public:
			D3D12Pipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
			);
			D3D12Pipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
			);
			D3D12Pipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
			);
			~D3D12Pipeline();

			inline ID3D12PipelineState* GetD3D12PipelineState() const { return _PipelineState; }
			inline ID3D12PipelineState*& GetD3D12PipelineState() { return _PipelineState; }
			const D3D12RootSignature& GetD3D12RootSignature() const;
			inline const D3D12_PRIMITIVE_TOPOLOGY& GetD3D12PrimitiveTopology() const { return _PrimitiveTopology; }
			inline D3D12_PRIMITIVE_TOPOLOGY& GetD3D12PrimitiveTopology() { return _PrimitiveTopology; }
			D3D12Pipeline& operator=(_In_ const D3D12Pipeline& InPipeline);

		private:
			ID3D12PipelineState*	_PipelineState		= nullptr;
			D3D_PRIMITIVE_TOPOLOGY	_PrimitiveTopology	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		};
	}
}
