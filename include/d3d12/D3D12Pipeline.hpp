#pragma once

#include "Graphics/Pipeline.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class D3D12RootSignature;

		class D3D12Pipeline final : public Pipeline
		{
		public:
			D3D12Pipeline(
				_In_ Device& InDevice,
				_In_ const PipelineCreateInformation& InPipelineCreateInformation
			);

			inline ID3D12PipelineState* GetD3D12PipelineState() const { return _PipelineState; }
			const D3D12RootSignature& GetRootSignature() const { return _RootSignature; }
			const D3D12_PRIMITIVE_TOPOLOGY& GetD3D12PrimitiveTopology() const { return _PrimitiveTopology; }

		private:
			D3D12RootSignature&		_RootSignature;
			ID3D12PipelineState*	_PipelineState		= nullptr;
			D3D_PRIMITIVE_TOPOLOGY	_PrimitiveTopology	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		};
	}
}
