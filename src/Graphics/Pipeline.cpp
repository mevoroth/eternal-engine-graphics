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
			_In_ Shader* InVertex,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineRootSignature(InRootSignature)
			, PipelineInputLayout(InInputLayout)
			, PipelineRenderPass(InRenderPass)
			, ShaderVertex(InVertex)
			, ShaderPixel(InPixel)
			, PipelineDepthStencil(InDepthStencil)
			, PipelineRasterizer(InRasterizer)
			, PipelinePrimitiveTopology(InPrimitiveTopology)
		{
		}

		GraphicsPipelineCreateInformation::GraphicsPipelineCreateInformation(
			_In_ RootSignature& InRootSignature,
			_In_ InputLayout* InInputLayout,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InVertex,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineCreateInformation(
				InRootSignature,
				InInputLayout,
				InRenderPass,
				InVertex,
				InPixel,
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
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH_AMPLIFICATION_PIXEL;
		}

		//MeshPipelineCreateInformation::MeshPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
		//	: PipelineCreateInformation()
		//{
		//	ETERNAL_BREAK(); // Not implemented
		//	PipelineRecreate = true;
		//}

		//////////////////////////////////////////////////////////////////////////
		// RayTracing
		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature& InGlobalRootSignature,
			_In_ Shader* InRayTracingRayGeneration,
			_In_ Shader* InRayTracingClosestHit,
			_In_ Shader* InRayTracingMiss,
			_In_ Shader* InRayTracingAnyHit
		)
			: PipelineRootSignature(InGlobalRootSignature)
			, ShaderRayTracingRayGeneration(InRayTracingRayGeneration)
			, ShaderRayTracingClosestHit(InRayTracingClosestHit)
			, ShaderRayTracingMiss(InRayTracingMiss)
			, ShaderRayTracingAnyHit(InRayTracingAnyHit)
		{
		}

		RayTracingPipelineCreateInformation::RayTracingPipelineCreateInformation(
			_In_ RootSignature& InGlobalRootSignature,
			_In_ Shader* InRayTracingRayGeneration,
			_In_ Shader* InRayTracingClosestHit,
			_In_ Shader* InRayTracingMiss,
			_In_ Shader* InRayTracingAnyHit
		)
			: PipelineCreateInformation(
				InGlobalRootSignature,
				InRayTracingRayGeneration,
				InRayTracingClosestHit,
				InRayTracingMiss,
				InRayTracingAnyHit
			)
		{
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_NONE;

			if (InRayTracingRayGeneration)
				PipelineShaderTypes |= ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION;

			if (InRayTracingClosestHit)
				PipelineShaderTypes |= ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_CLOSEST_HIT;

			if (InRayTracingMiss)
				PipelineShaderTypes |= ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_MISS;

			if (InRayTracingAnyHit)
				PipelineShaderTypes |= ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ANY_HIT;
		}

		RayTracingPipelineCreateInformation::RayTracingPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: RayTracingPipelineCreateInformation(
				InPipelineCreateInformation.PipelineRootSignature,
				InPipelineCreateInformation.ShaderRayTracingRayGeneration,
				InPipelineCreateInformation.ShaderRayTracingClosestHit,
				InPipelineCreateInformation.ShaderRayTracingMiss,
				InPipelineCreateInformation.ShaderRayTracingAnyHit
			)
		{
			ETERNAL_ASSERT((InPipelineCreateInformation.PipelineShaderTypes & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION);
			PipelineRecreated = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// PipelineCreateInformation
		PipelineCreateInformation::PipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
			: PipelineRootSignature(InPipelineCreateInformation.PipelineRootSignature)
			, PipelineInputLayout(InPipelineCreateInformation.PipelineInputLayout)
			, PipelineRenderPass(InPipelineCreateInformation.PipelineRenderPass)
			, ShaderVertex(InPipelineCreateInformation.ShaderVertex)
			, ShaderHull(InPipelineCreateInformation.ShaderHull)
			, ShaderDomain(InPipelineCreateInformation.ShaderDomain)
			, ShaderGeometry(InPipelineCreateInformation.ShaderGeometry)
			, ShaderPixel(InPipelineCreateInformation.ShaderPixel)
			, ShaderCompute(InPipelineCreateInformation.ShaderCompute)
			, ShaderMesh(InPipelineCreateInformation.ShaderMesh)
			, ShaderAmplification(InPipelineCreateInformation.ShaderAmplification)
			, ShaderRayTracingRayGeneration(InPipelineCreateInformation.ShaderRayTracingRayGeneration)
			, ShaderRayTracingClosestHit(InPipelineCreateInformation.ShaderRayTracingClosestHit)
			, ShaderRayTracingMiss(InPipelineCreateInformation.ShaderRayTracingMiss)
			, ShaderRayTracingAnyHit(InPipelineCreateInformation.ShaderRayTracingAnyHit)
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

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_HULL) == ShaderTypeFlags::SHADER_TYPE_FLAGS_HULL && InPipelineCreateInformation.ShaderHull)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderHull);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_DOMAIN) == ShaderTypeFlags::SHADER_TYPE_FLAGS_DOMAIN && InPipelineCreateInformation.ShaderDomain)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderDomain);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_GEOMETRY) == ShaderTypeFlags::SHADER_TYPE_FLAGS_GEOMETRY && InPipelineCreateInformation.ShaderGeometry)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderGeometry);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL) == ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL && InPipelineCreateInformation.ShaderPixel)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderPixel);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH) == ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH && InPipelineCreateInformation.ShaderMesh)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderMesh);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION && InPipelineCreateInformation.ShaderAmplification)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderAmplification);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION && InPipelineCreateInformation.ShaderRayTracingRayGeneration)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderRayTracingRayGeneration);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_CLOSEST_HIT) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_CLOSEST_HIT && InPipelineCreateInformation.ShaderRayTracingClosestHit)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderRayTracingClosestHit);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_MISS) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_MISS && InPipelineCreateInformation.ShaderRayTracingMiss)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderRayTracingMiss);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ANY_HIT) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ANY_HIT && InPipelineCreateInformation.ShaderRayTracingAnyHit)
				InOutContext.GetPipelineDependency().RegisterPipelineDependency(this, InPipelineCreateInformation.ShaderRayTracingAnyHit);
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
