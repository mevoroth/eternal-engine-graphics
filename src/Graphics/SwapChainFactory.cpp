#include "Graphics/SwapChainFactory.hpp"

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
			case D3D12:
				return new D3D12SwapChain(DeviceObj, WindowObj, CommandQueueObj);

			case VULKAN:
				return new VulkanSwapChain(DeviceObj, WindowObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
