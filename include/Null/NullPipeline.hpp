#pragma once

#include "Graphics/Pipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullPipeline final : public Pipeline
		{
		public:

			NullPipeline(_Inout_ GraphicsContext& InOutContext)
				: Pipeline(InOutContext)
			{
			}

			NullPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			NullPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			NullPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			NullPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation
			)
				: Pipeline(InOutContext, InPipelineCreateInformation)
			{
			}

			virtual bool IsPipelineCompiled() const override final { return true; }
			virtual void SerializePipeline(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile) override final {}

		};
	}
}
