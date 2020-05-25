#ifndef _PIPELINE_HPP_
#define _PIPELINE_HPP_

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
				: RootSignature(InRootSignature)
				, InputLayout(InInputLayout)
				, RenderPass(InRenderPass)
				, VS(InVS)
				, PS(InPS)
				, DepthStencil(InDepthStencil)
			{
			}

			RootSignature& RootSignature;
			InputLayout& InputLayout;
			RenderPass& RenderPass;
			Shader& VS;
			Shader& PS;
			const DepthStencil& DepthStencil;
		};

		class Pipeline
		{
		public:
			Pipeline(const PipelineCreateInformation& CreateInformation)
				: _CreateInformation(CreateInformation)
			{
			}
			virtual ~Pipeline() {}

		private:
			PipelineCreateInformation _CreateInformation;
		};
	}
}

#endif
