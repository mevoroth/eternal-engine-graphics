#include "Graphics/Pipeline.hpp"

#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ InputLayout& InInputLayout,
			_In_ RenderPass& InRenderPass,
			_In_ Shader& InVS,
			_In_ Shader& InPS,
			_In_ const DepthStencil& InDepthStencil,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineInputLayout(InInputLayout)
			, PipelineRenderPass(InRenderPass)
			, VS(InVS)
			, PS(InPS)
			, PipelineDepthStencil(InDepthStencil)
			, PipelinePrimitiveTopology(InPrimitiveTopology)
			, PipelineShaderTypes(ShaderTypeFlags::VS | ShaderTypeFlags::PS)
		{
		}
	}
}
