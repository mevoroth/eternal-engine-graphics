#include "Graphics/SwapChainFactory.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "Vulkan/VulkanSwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain* CreateSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef _WIN32_WINNT >= _WIN32_WINNT_WIN10
			case D3D12:
				return new D3D12SwapChain(DeviceObj, WindowObj, CommandQueueObj);
#endif

			case VULKAN:
				return new VulkanSwapChain(DeviceObj, WindowObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
