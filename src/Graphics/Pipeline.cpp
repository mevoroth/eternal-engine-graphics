#include "Graphics/Pipeline.hpp"

#include "Graphics/ShaderType.hpp"
#include "Graphics/GraphicsContext.hpp"

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

		GraphicsPipelineCreateInformation::GraphicsPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: GraphicsPipelineCreateInformation(
				InPipelineCreateInformation.PipelineRootSignature,
				InPipelineCreateInformation.PipelineInputLayout,
				InPipelineCreateInformation.PipelineRenderPass,
				InPipelineCreateInformation.VS,
				InPipelineCreateInformation.PS,
				InPipelineCreateInformation.PipelineDepthStencil,
				InPipelineCreateInformation.PipelineRasterizer,
				InPipelineCreateInformation.PipelinePrimitiveTopology
			)
		{
			ETERNAL_ASSERT((InPipelineCreateInformation.PipelineShaderTypes & ShaderTypeFlags::VS) == ShaderTypeFlags::VS);
			PipelineRecreated = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// Compute

		PipelineCreateInformation::PipelineCreateInformation(_In_ RootSignature& InRootSignature, _In_ Shader* InCS)
			: PipelineRootSignature(InRootSignature)
			, CS(InCS)
		{
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation(_In_ RootSignature& InRootSignature, _In_ Shader* InCS)
			: PipelineCreateInformation(InRootSignature, InCS)
		{
			PipelineShaderTypes = ShaderTypeFlags::CS;
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: ComputePipelineCreateInformation(
				InPipelineCreateInformation.PipelineRootSignature,
				InPipelineCreateInformation.CS
			)
		{
			ETERNAL_ASSERT(InPipelineCreateInformation.PipelineShaderTypes == ShaderTypeFlags::CS);
			PipelineRecreated = true;
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

		//MeshPipelineCreateInformation::MeshPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
		//	: PipelineCreateInformation()
		//{
		//	ETERNAL_BREAK(); // Not implemented
		//	PipelineRecreate = true;
		//}

		//////////////////////////////////////////////////////////////////////////
		// PipelineCreateInformation
		PipelineCreateInformation::PipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: PipelineRootSignature(InPipelineCreateInformation.PipelineRootSignature)
			, PipelineInputLayout(InPipelineCreateInformation.PipelineInputLayout)
			, PipelineRenderPass(InPipelineCreateInformation.PipelineRenderPass)
			, VS(InPipelineCreateInformation.VS)
			//, HS(InPipelineCreateInformation.HS)
			//, DS(InPipelineCreateInformation.DS)
			//, GS(InPipelineCreateInformation.GS)
			, PS(InPipelineCreateInformation.PS)
			, CS(InPipelineCreateInformation.CS)
			, MS(InPipelineCreateInformation.MS)
			, AS(InPipelineCreateInformation.AS)
			, PipelineRasterizer(InPipelineCreateInformation.PipelineRasterizer)
			, PipelineDepthStencil(InPipelineCreateInformation.PipelineDepthStencil)
			, PipelinePrimitiveTopology(InPipelineCreateInformation.PipelinePrimitiveTopology)
			, PipelineShaderTypes(InPipelineCreateInformation.PipelineShaderTypes)
		{
		}

		PipelineCreateInformation& PipelineCreateInformation::operator=(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
		{
			if (this != &InPipelineCreateInformation)
			{
				new (this) PipelineCreateInformation(InPipelineCreateInformation);
			}
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		// Pipeline
		Pipeline::Pipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: _PipelineCreateInformation(InPipelineCreateInformation)
		{
			ETERNAL_ASSERT(GetShaderTypes() != ShaderTypeFlags::SHADER_TYPE_FLAGS_UNDEFINED);

			if (InPipelineCreateInformation.IsPipelineRecreated())
				return;

			if (GetShaderTypes() == ShaderTypeFlags::CS)
			{
				ETERNAL_ASSERT(InPipelineCreateInformation.CS);
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.CS);
				return;
			}

			if ((GetShaderTypes() & ShaderTypeFlags::VS) == ShaderTypeFlags::VS && InPipelineCreateInformation.VS)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.VS);

			//if ((GetShaderTypes() & ShaderTypeFlags::HS) == ShaderTypeFlags::HS && InPipelineCreateInformation.HS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.HS);

			//if ((GetShaderTypes() & ShaderTypeFlags::DS) == ShaderTypeFlags::DS && InPipelineCreateInformation.DS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.DS);

			//if ((GetShaderTypes() & ShaderTypeFlags::GS) == ShaderTypeFlags::GS && InPipelineCreateInformation.GS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.GS);

			if ((GetShaderTypes() & ShaderTypeFlags::PS) == ShaderTypeFlags::PS && InPipelineCreateInformation.PS)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.PS);

			if ((GetShaderTypes() & ShaderTypeFlags::MS) == ShaderTypeFlags::MS && InPipelineCreateInformation.MS)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.MS);

			if ((GetShaderTypes() & ShaderTypeFlags::AS) == ShaderTypeFlags::AS && InPipelineCreateInformation.AS)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.AS);
		}

		Pipeline& Pipeline::operator=(_In_ const Pipeline& InPipeline)
		{
			if (this != &InPipeline)
			{
				_PipelineCreateInformation = InPipeline._PipelineCreateInformation;
			}
			return *this;
		}
	}
}
