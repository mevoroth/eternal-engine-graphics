#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class RootSignature;
		class InputLayout;
		class RenderPass;
		class Shader;
		class DepthStencil;
		class Viewport;

		struct PipelineCreateInformation
		{
			PipelineCreateInformation(
				RootSignature& InRootSignature,
				InputLayout& InInputLayout,
				RenderPass& InRenderPass,
				Shader& InVS,
				Shader& InPS,
				const DepthStencil& InDepthStencil
			)
				: PipelineRootSignature(InRootSignature)
				, PipelineInputLayout(InInputLayout)
				, PipelineRenderPass(InRenderPass)
				, VS(InVS)
				, PS(InPS)
				, PipelineDepthStencil(InDepthStencil)
			{
			}

			RootSignature& PipelineRootSignature;
			InputLayout& PipelineInputLayout;
			RenderPass& PipelineRenderPass;
			Shader& VS;
			Shader& PS;
			const DepthStencil& PipelineDepthStencil;
		};

		class Pipeline
		{
		public:
			Pipeline(const PipelineCreateInformation& InPipelineCreateInformation)
				: _PipelineCreateInformation(InPipelineCreateInformation)
			{
			}
			virtual ~Pipeline() {}

		private:
			PipelineCreateInformation _PipelineCreateInformation;
		};
	}
}
