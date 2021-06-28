#pragma once

#include "Graphics/Pipeline.hpp"

struct ID3D12PipelineState;
struct ID3D12RootSignature;

namespace Eternal
{
	namespace Graphics
	{
		class InputLayout;
		class Device;
		class RootSignature;

		class D3D12Pipeline : public Pipeline
		{
		public:
			D3D12Pipeline(
				_In_ Device& InDevice,
				_In_ const PipelineCreateInformation& InPipelineCreateInformation
			);

			inline ID3D12PipelineState* GetD3D12PipelineState() { return _PipelineState; }
			RootSignature& GetRootSignature() { return _RootSignature; }

		private:
			RootSignature&			_RootSignature;
			ID3D12PipelineState*	_PipelineState = nullptr;
		};
	}
}
