#include "Graphics/SwapChainFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullSwapChain.hpp"
#include "Windows/d3d12/WindowsD3D12SwapChain.hpp"
#include "Windows/Vulkan/WindowsVulkanSwapChain.hpp"
#include "Android/Vulkan/AndroidVulkanSwapChain.hpp"
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

#if ETERNAL_PLATFORM_WINDOWS
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12WindowsSwapChain(InContext);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
			{
				WindowsVulkanSwapChain* NewSwapChain = new WindowsVulkanSwapChain(InContext);
				NewSwapChain->InitializeVulkanSwapChain(InContext);
				return NewSwapChain;
			}
#endif
#endif

#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
			{
				AndroidVulkanSwapChain* NewSwapChain = new AndroidVulkanSwapChain(InContext);
				NewSwapChain->InitializeVulkanSwapChain(InContext);
				return NewSwapChain;
			}
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
