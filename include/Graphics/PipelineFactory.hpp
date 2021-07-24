#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class GraphicsContext;
		struct PipelineCreateInformation;

		Pipeline* CreatePipeline(
			_In_ GraphicsContext& InContext,
			_In_ const PipelineCreateInformation& InPipelineCreateInformation
		);
		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline);
	}
}
