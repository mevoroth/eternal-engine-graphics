#include "Graphics/RenderPassFactory.hpp"

#include "NextGenGraphics/Context.hpp"
#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "d3d12/D3D12RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RenderPass* CreateRenderPass(_In_ GraphicsContext& Context, _In_ const RenderPassCreateInformation& CreateInformation)
		{
			switch (Context.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12RenderPass(CreateInformation);
#endif

			case DeviceType::VULKAN:
				return new VulkanRenderPass(Context, CreateInformation);

			default:
				break;
			}
			return nullptr;
		}
	}
}
