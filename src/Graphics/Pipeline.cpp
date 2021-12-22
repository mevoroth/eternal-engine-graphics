#include "Graphics/Pipeline.hpp"

#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////
		// Graphics

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ InputLayout* InInputLayout,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InVS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineInputLayout(InInputLayout)
			, PipelineRenderPass(InRenderPass)
			, VS(InVS)
			, PS(InPS)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
			, PipelinePrimitiveTopology(InPrimitiveTopology)
		{
		}

		GraphicsPipelineCreateInformation::GraphicsPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ InputLayout* InInputLayout,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InVS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InInputLayout,
				InRenderPass,
				InVS,
				InPS,
				InDepthStencil,
				InRasterizer,
				InPrimitiveTopology
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::VS_PS;
		}

		//////////////////////////////////////////////////////////////////////////
		// Compute

		PipelineCreateInformation::PipelineCreateInformation( _In_ RootSignature& InRootSignature, _In_ Shader* InCS )
			: PipelineRootSignature(InRootSignature)
			, CS(InCS)
		{
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation( _In_ RootSignature& InRootSignature, _In_ Shader* InCS )
			: PipelineCreateInformation(InRootSignature, InCS)
		{
			PipelineShaderTypes = ShaderTypeFlags::CS;
		}

		//////////////////////////////////////////////////////////////////////////
		// Mesh

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, MS(InMS)
			, PS(InPS)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
		{
		}

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMS,
			_In_ Shader* InAS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, MS(InMS)
			, AS(InAS)
			, PS(InPS)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
		{
		}

		MeshPipelineCreateInformation::MeshPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InRenderPass,
				InMS,
				InPS,
				InDepthStencil,
				InRasterizer
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::MS_PS;
		}

		MeshPipelineCreateInformation::MeshPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMS,
			_In_ Shader* InAS,
			_In_ Shader* InPS,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InRenderPass,
				InMS,
				InAS,
				InPS,
				InDepthStencil,
				InRasterizer
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::MS_AS_PS;
		}
	}
}
