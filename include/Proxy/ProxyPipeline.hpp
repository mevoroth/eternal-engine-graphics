#pragma once

#include "Graphics/Pipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class ProxyPipeline final : public Pipeline
		{
		public:

			ProxyPipeline(_Inout_ GraphicsContext& InOutContext)
				: Pipeline(InOutContext)
			{
			}

			template<typename PipelineCreateInformationType>
			ProxyPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const PipelineCreateInformationType& InPipelineCreateInformation
			);

			virtual bool IsPipelineCompiled() const override final;
			virtual void SerializePipeline(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile) override final;

		private:

			Pipeline* _Pipeline = nullptr;

		};
	}
}
