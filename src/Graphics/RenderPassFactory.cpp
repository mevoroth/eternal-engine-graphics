#include "Graphics/RenderPassFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "d3d12/D3D12RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RenderPass* CreateRenderPass(_In_ Device& DeviceObj, _In_ const Viewport& ViewportObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ View* DepthStencil /* = nullptr */, _In_ const LogicBlend& LogicBlendObj /* = LogicBlend() */)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12RenderPass(ViewportObj, RenderTargets, BlendStates, DepthStencil, LogicBlendObj);
#endif

			case VULKAN:
				return new VulkanRenderPass(DeviceObj, ViewportObj, RenderTargets, BlendStates, DepthStencil, LogicBlendObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
