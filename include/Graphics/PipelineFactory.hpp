#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class GraphicsContext;
		struct GraphicsPipelineCreateInformation;
		struct ComputePipelineCreateInformation;
		struct MeshPipelineCreateInformation;

		Pipeline* CreatePipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
		);
		Pipeline* CreatePipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		);
		Pipeline* CreatePipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
		);
		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline);

		void SwapPipelines(_In_ GraphicsContext& InContext, _Inout_ Pipeline* A, _Inout_ Pipeline* B);
	}
}
