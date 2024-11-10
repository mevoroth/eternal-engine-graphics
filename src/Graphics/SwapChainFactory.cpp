#include "Graphics/SwapChainFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullSwapChain.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/SwapChainFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		SwapChain* CreateSwapChain(_In_ GraphicsContext& InContext)
		{
			LogWrite(LogInfo, LogGraphics, "[Graphics::CreateDevice]Creating SwapChain");
			switch (InContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullSwapChain(InContext);

#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12SwapChain(InContext);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanSwapChain(InContext);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateSwapChainPrivate(InContext);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
