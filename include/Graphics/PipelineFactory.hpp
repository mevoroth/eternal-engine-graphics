#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class GraphicsContext;
		struct GraphicsPipelineCreateInformation;
		struct ComputePipelineCreateInformation;

		Pipeline* CreatePipeline(
			_In_ GraphicsContext& InContext,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
		);
		Pipeline* CreatePipeline(
			_In_ GraphicsContext& InContext,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		);

		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline);
	}
}
