#pragma once

#include "Graphics/DepthStencil.hpp"
#include "Graphics/Rasterizer.hpp"
#include "Graphics/RenderPass.hpp"
#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace Graphics
	{
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
			RootSignature&		PipelineRootSignature;
			InputLayout*		PipelineInputLayout			= nullptr;
			RenderPass*			PipelineRenderPass			= nullptr;
			Shader*				VS							= nullptr;
			//Shader*				HS							= nullptr;
			//Shader*				DS							= nullptr;
			//Shader*				GS							= nullptr;
			Shader*				PS							= nullptr;
			Shader*				CS							= nullptr;
			Shader*				MS							= nullptr;
			Shader*				AS							= nullptr;
			Rasterizer			PipelineRasterizer;
			DepthStencil		PipelineDepthStencil;
			PrimitiveTopology	PipelinePrimitiveTopology	= PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			ShaderTypeFlags		PipelineShaderTypes			= ShaderTypeFlags::SHADER_TYPE_FLAGS_UNDEFINED;

		protected:

			PipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ InputLayout* InInputLayout,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InVS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault,
				_In_ const PrimitiveTopology& InPrimitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			);

			PipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ Shader* InCS
			);

			PipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			PipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMS,
				_In_ Shader* InAS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);
		};

		struct GraphicsPipelineCreateInformation : public PipelineCreateInformation
		{
			GraphicsPipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ InputLayout* InInputLayout,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InVS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault,
				_In_ const PrimitiveTopology& InPrimitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			);
		};

		struct ComputePipelineCreateInformation : public PipelineCreateInformation
		{
			ComputePipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ Shader* InCS
			);
		};

		struct MeshPipelineCreateInformation : public PipelineCreateInformation
		{
			MeshPipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);

			MeshPipelineCreateInformation(
				_In_ RootSignature& InRootSignature,
				_In_ RenderPass* InRenderPass,
				_In_ Shader* InMS,
				_In_ Shader* InAS,
				_In_ Shader* InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault
			);
		};

		class Pipeline
		{
		public:
			virtual ~Pipeline() {}

			const Viewport& GetViewport() const { return static_cast<const RenderPass*>(_PipelineCreateInformation.PipelineRenderPass)->GetViewport(); }
			const ShaderTypeFlags& GetShaderTypes() const { return _PipelineCreateInformation.PipelineShaderTypes; }
			const RootSignature& GetRootSignature() const { return _PipelineCreateInformation.PipelineRootSignature; }

		protected:
			Pipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformation& InPipelineCreateInformation);

		private:
			PipelineCreateInformation _PipelineCreateInformation;
		};
	}
}
