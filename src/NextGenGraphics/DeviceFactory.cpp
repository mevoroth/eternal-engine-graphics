#include "NextGenGraphics/DeviceFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Device.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ Window& WindowObj)
		{
			switch (DeviceTypeObj)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12Device(0);
#endif

			case VULKAN:
				return new VulkanDevice(WindowObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
