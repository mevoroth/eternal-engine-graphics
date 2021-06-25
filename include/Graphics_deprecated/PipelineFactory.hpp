#ifndef _PIPELINE_FACTORY_HPP_
#define _PIPELINE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class GraphicsContext;
		struct PipelineCreateInformation;

		Pipeline* CreatePipeline(
			_In_ GraphicsContext& Context,
			_In_ const PipelineCreateInformation& CreateInformation
		);
	}
}

#endif
