#include "Graphics/Pipeline.hpp"

#include "Graphics/ShaderType.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Shader.hpp"
#include "File/File.hpp"

namespace Eternal
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////
		// Graphics

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature* InRootSignature,
			_In_ InputLayout* InInputLayout,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InVertex,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */,
			_In_ const PrimitiveTopology& InPrimitiveTopology /* = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		)
			: PipelineName("Vertex_" + string(InVertex->GetName()) + (InPixel ? " Pixel_" + string(InPixel->GetName()) : "_Only"))
			, PipelineRootSignature(InRootSignature)
			, PipelineInputLayout(InInputLayout)
			, PipelineRenderPass(InRenderPass)
			, ShaderVertex(InVertex)
			, ShaderPixel(InPixel)
			, PipelineRasterizer(InRasterizer)
			, PipelineDepthStencil(InDepthStencil)
			, PipelinePrimitiveTopology(InPrimitiveTopology)
		{
		}

		GraphicsPipelineCreateInformation::GraphicsPipelineCreateInformation(
			_In_ RootSignature* InRootSignature,
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
			PipelineShaderTypes = ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX;
			if (InPixel)
				PipelineShaderTypes |= ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL;
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

		PipelineCreateInformation::PipelineCreateInformation(_In_ RootSignature* InRootSignature, _In_ Shader* InCompute)
			: PipelineName("Compute_" + string(InCompute->GetName()))
			, PipelineRootSignature(InRootSignature)
			, ShaderCompute(InCompute)
		{
		}

		ComputePipelineCreateInformation::ComputePipelineCreateInformation(_In_ RootSignature* InRootSignature, _In_ Shader* InCompute)
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
			_In_ RootSignature* InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineName("Mesh_" + string(InMesh->GetName()) + (InPixel ? " Pixel_" + string(InPixel->GetName()) : "_Only"))
			, PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, ShaderPixel(InPixel)
			, ShaderMesh(InMesh)
			, PipelineRasterizer(InRasterizer)
			, PipelineDepthStencil(InDepthStencil)
		{
		}

		PipelineCreateInformation::PipelineCreateInformation(
			_In_ RootSignature* InRootSignature,
			_In_ RenderPass* InRenderPass,
			_In_ Shader* InMesh,
			_In_ Shader* InAmplification,
			_In_ Shader* InPixel,
			_In_ const DepthStencil& InDepthStencil /* = Graphics::DepthStencilNoneNone */,
			_In_ const Rasterizer& InRasterizer /* = RasterizerDefault */
		)
			: PipelineName(	"Mesh_" + string(InMesh->GetName())
							+ (InAmplification ?	" Amplification_" + string(InAmplification->GetName())	: string())
							+ (InPixel ?			" Pixel_" + string(InPixel->GetName())					: "_Only"))
			, PipelineRootSignature(InRootSignature)
			, PipelineRenderPass(InRenderPass)
			, ShaderPixel(InPixel)
			, ShaderMesh(InMesh)
			, ShaderAmplification(InAmplification)
			, PipelineRasterizer(InRasterizer)
			, PipelineDepthStencil(InDepthStencil)
		{
		}

		MeshPipelineCreateInformation::MeshPipelineCreateInformation(
			_In_ RootSignature* InRootSignature,
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
			_In_ RootSignature* InRootSignature,
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
			_In_ RootSignature* InGlobalRootSignature,
			_In_ Shader* InRayTracingRayGeneration,
			_In_ Shader* InRayTracingClosestHit,
			_In_ Shader* InRayTracingMiss,
			_In_ Shader* InRayTracingAnyHit
		)
			: PipelineName(	"RayGeneration_" + string(InRayTracingRayGeneration->GetName())
							+ (InRayTracingClosestHit	? " ClosestHit_" + string(InRayTracingClosestHit->GetName())	: string())
							+ (InRayTracingMiss			? " Miss_" + string(InRayTracingMiss->GetName())				: string())
							+ (InRayTracingAnyHit		? " AnyHit_" + string(InRayTracingAnyHit->GetName())			: string()))
			, PipelineRootSignature(InGlobalRootSignature)
			, ShaderRayTracingRayGeneration(InRayTracingRayGeneration)
			, ShaderRayTracingClosestHit(InRayTracingClosestHit)
			, ShaderRayTracingMiss(InRayTracingMiss)
			, ShaderRayTracingAnyHit(InRayTracingAnyHit)
		{
		}

		RayTracingPipelineCreateInformation::RayTracingPipelineCreateInformation(
			_In_ RootSignature* InGlobalRootSignature,
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
			: PipelineName(InPipelineCreateInformation.PipelineName)
			, PipelineRootSignature(InPipelineCreateInformation.PipelineRootSignature)
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
		Pipeline::Pipeline(_Inout_ GraphicsContext& InOutContext)
		{
			(void)InOutContext;
		}

		Pipeline::Pipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformation& InPipelineCreateInformation, _In_ bool IsSkippingRegistration)
			: _PipelineCreateInformation(InPipelineCreateInformation)
		{
			ETERNAL_ASSERT(GetShaderTypes() != ShaderTypeFlags::SHADER_TYPE_FLAGS_UNDEFINED);

			if (InPipelineCreateInformation.IsPipelineRecreated() || IsSkippingRegistration)
				return;

			_RegisterPipeline(InOutContext);
		}

		void Pipeline::SerializePipeline(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile)
		{
			(void)InOutContext;
			InOutFile->Serialize(_PipelineCreateInformation.PipelineName);
			InOutFile->Serialize(_PipelineCreateInformation.PipelineShaderTypes);
		}

		void Pipeline::_RegisterPipeline(_Inout_ GraphicsContext& InOutContext)
		{
			if (GetShaderTypes() == ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE)
			{
				ETERNAL_ASSERT(_PipelineCreateInformation.ShaderCompute);
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderCompute);
				return;
			}

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX) == ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX && _PipelineCreateInformation.ShaderVertex)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderVertex);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_HULL) == ShaderTypeFlags::SHADER_TYPE_FLAGS_HULL && _PipelineCreateInformation.ShaderHull)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderHull);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_DOMAIN) == ShaderTypeFlags::SHADER_TYPE_FLAGS_DOMAIN && _PipelineCreateInformation.ShaderDomain)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderDomain);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_GEOMETRY) == ShaderTypeFlags::SHADER_TYPE_FLAGS_GEOMETRY && _PipelineCreateInformation.ShaderGeometry)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderGeometry);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL) == ShaderTypeFlags::SHADER_TYPE_FLAGS_PIXEL && _PipelineCreateInformation.ShaderPixel)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderPixel);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH) == ShaderTypeFlags::SHADER_TYPE_FLAGS_MESH && _PipelineCreateInformation.ShaderMesh)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderMesh);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_AMPLIFICATION && _PipelineCreateInformation.ShaderAmplification)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderAmplification);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION && _PipelineCreateInformation.ShaderRayTracingRayGeneration)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderRayTracingRayGeneration);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_CLOSEST_HIT) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_CLOSEST_HIT && _PipelineCreateInformation.ShaderRayTracingClosestHit)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderRayTracingClosestHit);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_MISS) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_MISS && _PipelineCreateInformation.ShaderRayTracingMiss)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderRayTracingMiss);

			if ((GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ANY_HIT) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ANY_HIT && _PipelineCreateInformation.ShaderRayTracingAnyHit)
				InOutContext.GetPipelineLibrary().RegisterPipelineDependency(this, _PipelineCreateInformation.ShaderRayTracingAnyHit);
		}

		void Pipeline::FinalizeConstruction(_In_ const PipelineCreateInformation& InPipelineCreateInformation)
		{
			_PipelineCreateInformation.PipelineRootSignature		= InPipelineCreateInformation.PipelineRootSignature;
			_PipelineCreateInformation.PipelineInputLayout			= InPipelineCreateInformation.PipelineInputLayout;
			_PipelineCreateInformation.PipelineRenderPass			= InPipelineCreateInformation.PipelineRenderPass;
			_PipelineCreateInformation.PipelineRasterizer			= InPipelineCreateInformation.PipelineRasterizer;
			_PipelineCreateInformation.PipelineDepthStencil			= InPipelineCreateInformation.PipelineDepthStencil;
			_PipelineCreateInformation.PipelinePrimitiveTopology	= InPipelineCreateInformation.PipelinePrimitiveTopology;
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
