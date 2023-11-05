#pragma once

#include "Graphics/Pipeline.hpp"

#if ETERNAL_ENABLE_D3D12

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class D3D12RootSignature;
		class Resource;

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

			D3D12Pipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation
			);

			~D3D12Pipeline();

			inline ID3D12PipelineState* GetD3D12PipelineState() const { return _Pipeline.PipelineState; }
			inline ID3D12PipelineState*& GetD3D12PipelineState() { return _Pipeline.PipelineState; }
			inline ID3D12StateObject* GetD3D12StateObject() const { return _Pipeline.StateObject; }
			inline ID3D12StateObject*& GetD3D12StateObject() { return _Pipeline.StateObject; }
			const D3D12RootSignature& GetD3D12RootSignature() const;
			inline const D3D12_PRIMITIVE_TOPOLOGY& GetD3D12PrimitiveTopology() const { return _PrimitiveTopology; }
			inline D3D12_PRIMITIVE_TOPOLOGY& GetD3D12PrimitiveTopology() { return _PrimitiveTopology; }
			D3D12Pipeline& operator=(_In_ const D3D12Pipeline& InPipeline);

			virtual bool IsPipelineCompiled() const override final;
			bool IsRayTracingPipeline() const;

		private:
			union D3D12PipelineInternal
			{
				D3D12PipelineInternal()
					: PipelineState(nullptr)
					, StateObject(nullptr)
				{
				}

				ID3D12PipelineState*	PipelineState;
				ID3D12StateObject*		StateObject;
			}							_Pipeline;
			D3D_PRIMITIVE_TOPOLOGY		_PrimitiveTopology	= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		};
	}
}

#endif
