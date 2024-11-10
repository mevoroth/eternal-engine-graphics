#include "Proxy/ProxyPipeline.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"
#include "Graphics/PipelineFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename PipelineCreateInformationType>
		ProxyPipeline::ProxyPipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformationType& InPipelineCreateInformation)
			: Pipeline(InOutContext, InPipelineCreateInformation, true)
			, _Pipeline(CreatePipeline(static_cast<ProxyGraphicsContext&>(InOutContext).GetProxyDeviceType(), InOutContext, InPipelineCreateInformation))
		{
		}

		bool ProxyPipeline::IsPipelineCompiled() const
		{
			return _Pipeline && _Pipeline->IsPipelineCompiled();
		}

		void ProxyPipeline::SerializePipeline(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile)
		{
			(void)InOutContext;
			(void)InOutFile;
		}

		template ProxyPipeline::ProxyPipeline(_Inout_ GraphicsContext& InOutContext, _In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation);
		template ProxyPipeline::ProxyPipeline(_Inout_ GraphicsContext& InOutContext, _In_ const ComputePipelineCreateInformation& InPipelineCreateInformation);
		template ProxyPipeline::ProxyPipeline(_Inout_ GraphicsContext& InOutContext, _In_ const MeshPipelineCreateInformation& InPipelineCreateInformation);
		template ProxyPipeline::ProxyPipeline(_Inout_ GraphicsContext& InOutContext, _In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation);
	}
}
