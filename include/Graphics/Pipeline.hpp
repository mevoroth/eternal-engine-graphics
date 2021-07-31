#pragma once

#include "Graphics/DepthStencil.hpp"
#include "Graphics/Rasterizer.hpp"
#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class ShaderTypeFlags;

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
				_In_ RootSignature& InRootSignature,
				_In_ InputLayout& InInputLayout,
				_In_ RenderPass& InRenderPass,
				_In_ Shader& InVS,
				_In_ Shader& InPS,
				_In_ const DepthStencil& InDepthStencil = Graphics::DepthStencilNoneNone,
				_In_ const Rasterizer& InRasterizer = RasterizerDefault,
				_In_ const PrimitiveTopology& InPrimitiveTopology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
			);

			RootSignature&		PipelineRootSignature;
			InputLayout&		PipelineInputLayout;
			RenderPass&			PipelineRenderPass;
			Shader&				VS;
			Shader&				PS;
			Rasterizer			PipelineRasterizer;
			DepthStencil		PipelineDepthStencil;
			PrimitiveTopology	PipelinePrimitiveTopology	= PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			ShaderTypeFlags		PipelineShaderTypes;
		};

		class Pipeline
		{
		public:
			virtual ~Pipeline() {}

			const Viewport& GetViewport() const { return static_cast<const RenderPass&>(_PipelineCreateInformation.PipelineRenderPass).GetViewport(); }
			const ShaderTypeFlags& GetShaderTypes() const { return _PipelineCreateInformation.PipelineShaderTypes; }
			const RootSignature& GetRootSignature() const { return _PipelineCreateInformation.PipelineRootSignature; }

		protected:
			Pipeline(const PipelineCreateInformation& InPipelineCreateInformation)
				: _PipelineCreateInformation(InPipelineCreateInformation)
			{
			}

		private:
			PipelineCreateInformation _PipelineCreateInformation;
		};
	}
}
