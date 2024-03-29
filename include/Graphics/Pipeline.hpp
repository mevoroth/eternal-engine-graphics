#pragma once

#include "Graphics/DepthStencil.hpp"
#include "Graphics/Rasterizer.hpp"
#include "Graphics/RenderPass.hpp"
#include "Graphics/ShaderType.hpp"
#include <string>

namespace Eternal
{
	namespace FileSystem
	{
		class File;
	}
	namespace Graphics
	{
		using namespace Eternal::FileSystem;

		enum class ShaderTypeFlags;

		class GraphicsContext;
		class RootSignature;
		class InputLayout;
		class Shader;

		enum class PrimitiveTopology
		{
			PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
			PRIMITIVE_TOPOLOGY_LINE_LIST,
			PRIMITIVE_TOPOLOGY_LINE_STRIP,
			PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			PRIMITIVE_TOPOLOGY_LINE_LIST_ADJACENCY,
			PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJACENCY,
			PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJACENCY,
			PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJACENCY,
			PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST,
			PRIMITIVE_TOPOLOGY_COUNT
		};

		struct PipelineCreateInformation
		{
			PipelineCreateInformation(
				_In_ const PipelineCreateInformation& InPipelineCreateInformation
			);

			PipelineCreateInformation& operator=(_In_ const PipelineCreateInformation& InPipelineCreateInformation);

			string				PipelineName;
			RootSignature*		PipelineRootSignature			= nullptr;
			InputLayout*		PipelineInputLayout				= nullptr;
			RenderPass*			PipelineRenderPass				= nullptr;
			Shader*				ShaderVertex					= nullptr;
			Shader*				ShaderHull						= nullptr;
			Shader*				ShaderDomain					= nullptr;
			Shader*				ShaderGeometry					= nullptr;
			Shader*				ShaderPixel						= nullptr;
			Shader*				ShaderCompute					= nullptr;
			Shader*				ShaderMesh						= nullptr;
			Shader*				ShaderAmplification				= nullptr;
			Shader*				ShaderRayTracingRayGeneration	= nullptr;
			Shader*				ShaderRayTracingClosestHit		= nullptr;
			Shader*				ShaderRayTracingMiss			= nullptr;
			Shader*				ShaderRayTracingAnyHit			= nullptr;
			Rasterizer			PipelineRasterizer;
			DepthStencil		PipelineDepthStencil;
			PrimitiveTopology	PipelinePrimitiveTopology		= PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			ShaderTypeFlags		PipelineShaderTypes				= ShaderTypeFlags::SHADER_TYPE_FLAGS_UNDEFINED;

			bool IsPipelineRecreated() const { return PipelineRecreated; }

		protected:

			PipelineCreateInformation() = default;

			PipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ InputLayout* InInputLayout,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InVertex,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault,
				_In_ const PrimitiveTopology& InPrimitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			);

			PipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ Shader* InCompute
			);

			PipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMesh,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			PipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMesh,
				_In_ Shader* InAmplification,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			PipelineCreateInformation(
				_In_ RootSignature* InGlobalRootSignature,
				_In_ Shader* InRayTracingRayGeneration,
				_In_ Shader* InRayTracingClosestHit,
				_In_ Shader* InRayTracingMiss,
				_In_ Shader* InRayTracingHit
			);

			bool PipelineRecreated = false;

			friend class Pipeline;
		};

		struct GraphicsPipelineCreateInformation : public PipelineCreateInformation
		{
			GraphicsPipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ InputLayout* InInputLayout,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InVertex,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault,
				_In_ const PrimitiveTopology& InPrimitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			);

			GraphicsPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation);
		};

		struct ComputePipelineCreateInformation : public PipelineCreateInformation
		{
			ComputePipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ Shader* InCompute
			);

			ComputePipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation);
		};

		struct MeshPipelineCreateInformation : public PipelineCreateInformation
		{
			MeshPipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMesh,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			MeshPipelineCreateInformation(
				_In_ RootSignature* InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMesh,
				_In_ Shader* InAmplification,
				_In_ Shader* InPixel,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			//MeshPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation);
		};

		struct RayTracingPipelineCreateInformation : public PipelineCreateInformation
		{
			RayTracingPipelineCreateInformation(
				_In_ RootSignature* InGlobalRootSignature,
				_In_ Shader* InRayTracingRayGeneration,
				_In_ Shader* InRayTracingClosestHit,
				_In_ Shader* InRayTracingMiss,
				_In_ Shader* InRayTracingAnyHit = nullptr
			);

			RayTracingPipelineCreateInformation(_In_ const PipelineCreateInformation& InPipelineCreateInformation);
		};

		class Pipeline
		{
		public:

			virtual ~Pipeline() {}
			virtual bool IsPipelineCompiled() const = 0;
			virtual void SerializePipeline(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile);

			const Viewport& GetViewport() const { return static_cast<const RenderPass*>(_PipelineCreateInformation.PipelineRenderPass)->GetViewport(); }
			const ShaderTypeFlags& GetShaderTypes() const { return _PipelineCreateInformation.PipelineShaderTypes; }
			const RootSignature* GetRootSignature() const { return _PipelineCreateInformation.PipelineRootSignature; }
			const PipelineCreateInformation& GetPipelineCreateInformation() const { return _PipelineCreateInformation; }
			PipelineCreateInformation& GetPipelineCreateInformation() { return _PipelineCreateInformation; }
			Pipeline& operator=(_In_ const Pipeline& InPipeline);

		protected:
			Pipeline(_Inout_ GraphicsContext& InOutContext);
			Pipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformation& InPipelineCreateInformation, _In_ bool IsSkippingRegistration = false);

			void _RegisterPipeline(_Inout_ GraphicsContext& InOutContext);

		private:

			void FinalizeConstruction(_In_ const PipelineCreateInformation& InPipelineCreateInformation);

			PipelineCreateInformation _PipelineCreateInformation;

			friend class PipelineLibrary;
		};
	}
}
