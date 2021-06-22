#include "Graphics/SwapChainFactory.hpp"

#include "Log/Log.hpp"
#include "NextGenGraphics/Context.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "Vulkan/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain* CreateSwapChain(_In_ GraphicsContext& Context)
		{
			LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating SwapChain");
			switch (Context.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12SwapChain(Context);
#endif
			case DeviceType::VULKAN:
				return new VulkanSwapChain(Context);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
