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
			_In_ GraphicsContext& InContext,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
		);
		Pipeline* CreatePipeline(
			_In_ GraphicsContext& InContext,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		);
		Pipeline* CreatePipeline(
			_In_ GraphicsContext& InContext,
			_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
		);
		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline);
	}
}
