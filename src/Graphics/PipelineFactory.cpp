#include "Graphics/PipelineFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "Vulkan/VulkanPipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
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
		)
		{
			switch (DeviceObj.GetDeviceType())
			{
			case D3D12:
				return new D3D12Pipeline(DeviceObj, RootSignatureObj, InputLayoutObj, RenderPassObj, VS, PS, DepthTestObj, StencilTestObj, ViewportObj);

			case VULKAN:
				return new VulkanPipeline(DeviceObj, RootSignatureObj, InputLayoutObj, RenderPassObj, VS, PS, DepthTestObj, StencilTestObj, ViewportObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
