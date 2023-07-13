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
			, ShaderVertex(InVS)
			, ShaderPixel(InPS)
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
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX_PIXEL;
		}

		GraphicsPipelineCreateInformation::GraphicsPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: GraphicsPipelineCreateInformation(
				InPipelineCreateInformation.PipelineRootSignature,
				InPipelineCreateInformation.PipelineInputLayout,
				InPipelineCreateInformation.PipelineRenderPass,
				InPipelineCreateInformation.ShaderVertex,
				InPipelineCreateInformation.ShaderPixel,
				InPipelineCreateInformation.PipelineDepthStencil,
				InPipelineCreateInformation.PipelineRasterizer,
				InPipelineCreateInformation.PipelinePrimitiveTopology
			)
		{
			ETERNAL_ASSERT((InPipelineCreateInformation.PipelineShaderTypes & ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX) == ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX);
			PipelineRecreated = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// Compute

		PipelineCreateInformation::PipelineCreateInformation(_In_ RootSignature& InRootSignature, _In_ Shader* InCompute)
			: PipelineRootSignature(InRootSignature)
			, ShaderCompute(InCompute)
		{
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation(_In_ RootSignature& InRootSignature, _In_ Shader* InCompute)
			: PipelineCreateInformation(InRootSignature, InCompute)
		{
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE;
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: ComputePipelineCreateInformation(
				InPipelineCreateInformation.PipelineRootSignature,
				InPipelineCreateInformation.ShaderCompute
			)
		{
			ETERNAL_ASSERT(InPipelineCreateInformation.PipelineShaderTypes == ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE);
			PipelineRecreated = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// Mesh

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, ShaderMesh(InMesh)
			, ShaderPixel(InPixel)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
		{
		}

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InAmplification,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, ShaderMesh(InMesh)
			, ShaderAmplification(InAmplification)
			, ShaderPixel(InPixel)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
		{
		}

		MeshPipelineCreateInformation::MeshPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InRenderPass,
				InMesh,
				InPixel,
				InDepthStencil,
				InRasterizer
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH_PIXEL;
		}

		MeshPipelineCreateInformation::MeshPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InAmplification,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InRenderPass,
				InMesh,
				InAmplification,
				InPixel,
				InDepthStencil,
				InRasterizer
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_MESH_AMPLIFICATION_PIXEL;
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
			, ShaderVertex(InPipelineCreateInformation.ShaderVertex)
			//, HS(InPipelineCreateInformation.HS)
			//, DS(InPipelineCreateInformation.DS)
			//, GS(InPipelineCreateInformation.GS)
			, ShaderPixel(InPipelineCreateInformation.ShaderPixel)
			, ShaderCompute(InPipelineCreateInformation.ShaderCompute)
			, ShaderMesh(InPipelineCreateInformation.ShaderMesh)
			, ShaderAmplification(InPipelineCreateInformation.ShaderAmplification)
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

			if (GetShaderTypes() == ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE)
			{
				ETERNAL_ASSERT(InPipelineCreateInformation.ShaderCompute);
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderCompute);
				return;
			}

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX) == ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX && InPipelineCreateInformation.ShaderVertex)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderVertex);

			//if ((GetShaderTypes() & ShaderTypeFlags::HS) == ShaderTypeFlags::HS && InPipelineCreateInformation.HS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.HS);

			//if ((GetShaderTypes() & ShaderTypeFlags::DS) == ShaderTypeFlags::DS && InPipelineCreateInformation.DS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.DS);

			//if ((GetShaderTypes() & ShaderTypeFlags::GS) == ShaderTypeFlags::GS && InPipelineCreateInformation.GS)
			//	InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.GS);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL) == ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL && InPipelineCreateInformation.ShaderPixel)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderPixel);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH) == ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH && InPipelineCreateInformation.ShaderMesh)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderMesh);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION && InPipelineCreateInformation.ShaderAmplification)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderAmplification);
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
