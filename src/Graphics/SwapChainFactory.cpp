#include "Graphics/SwapChainFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "Vulkan/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain* CreateSwapChain(_In_ GraphicsContext& InContext)
		{
			LogWrite(LogInfo, LogGraphics, "[Graphics::CreateDevice]Creating SwapChain");
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12SwapChain(InContext);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanSwapChain(InContext);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
