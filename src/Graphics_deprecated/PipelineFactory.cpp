#include "Graphics_deprecated/PipelineFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
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
			_In_ const Viewport& ViewportObj
		)
		{
			switch (DeviceObj.GetDeviceType())
			{
//#ifdef ETERNAL_ENABLE_D3D12
//			case DeviceType::D3D12:
//				return new D3D12Pipeline(DeviceObj, RootSignatureObj, InputLayoutObj, RenderPassObj, VS, PS, DepthTestObj, StencilTestObj, ViewportObj);
//#endif
//
//			case DeviceType::VULKAN:
//				return new VulkanPipeline(DeviceObj, RootSignatureObj, InputLayoutObj, RenderPassObj, VS, PS, DepthTestObj, StencilTestObj, ViewportObj);
//
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& Context, _In_ const PipelineCreateInformation& CreateInformation)
		{
			switch (Context.GetDevice().GetDeviceType())
			{
//#ifdef ETERNAL_ENABLE_D3D12
//			case DeviceType::D3D12:
//				return new D3D12Pipeline()
//#endif

			case DeviceType::VULKAN:
				return new VulkanPipeline(Context.GetDevice(), CreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
