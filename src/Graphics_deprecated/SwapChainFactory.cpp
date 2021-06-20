#include "Graphics_deprecated/SwapChainFactory.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include "Log/Log.hpp"
#include "NextGenGraphics/Context.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12SwapChain.hpp"
#include "Vulkan_deprecated/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain* CreateSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj)
		{
			LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating SwapChain");
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12SwapChain(DeviceObj, WindowObj, CommandQueueObj);
#endif

			case DeviceType::VULKAN:
				return new VulkanSwapChain(DeviceObj, WindowObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		SwapChain* CreateSwapChain(_In_ GraphicsContext& Context)
		{
			return CreateSwapChain(Context.GetDevice(), Context.GetWindow(), *(CommandQueue*)nullptr);
		}
	}
}
