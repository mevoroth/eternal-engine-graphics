#ifndef _PIPELINE_FACTORY_HPP_
#define _PIPELINE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class Device;
		class RootSignature;
		class InputLayout;
		class RenderPass;
		class Shader;
		class Viewport;
		class DepthTest;
		class StencilTest;

		Pipeline* CreatePipeline(
			_In_ Device& DeviceObj,
			_In_ RootSignature& RootSignatureObj,
			_In_ InputLayout& InputLayoutObj,
			_In_ RenderPass& RenderPassObj,
			_In_ Shader& VS,
			_In_ Shader& PS,
			_In_ const DepthTest& DepthTestObj,
			_In_ const StencilTest& StencilTestObj,
			_In_ Viewport& ViewportObj
		);
	}
}

#endif
