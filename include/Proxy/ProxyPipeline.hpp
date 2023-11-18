#pragma once

#include "Graphics/Pipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class ProxyPipeline final : public Pipeline
		{
		public:

			ProxyPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			ProxyPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			ProxyPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			ProxyPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			virtual bool IsPipelineCompiled() const override final { return true; }

		};
	}
}
