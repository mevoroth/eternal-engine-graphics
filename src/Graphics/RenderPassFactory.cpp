#include "Graphics/RenderPassFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RenderPass* CreateRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ View* DepthStencil /* = nullptr */)
		{
			switch (DeviceObj.GetDeviceType())
			{
			case D3D12:
				ETERNAL_ASSERT(false); // NOT IMPLEMENTED
				return nullptr;

			case VULKAN:
				return new VulkanRenderPass(DeviceObj, RenderTargets, DepthStencil);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
