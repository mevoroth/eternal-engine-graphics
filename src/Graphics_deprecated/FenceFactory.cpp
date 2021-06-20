#include "Graphics_deprecated/FenceFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12Fence.hpp"
#include "Vulkan_deprecated/VulkanFence.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Fence* CreateFence(_In_ Device& DeviceObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Fence(DeviceObj);
#endif

			case DeviceType::VULKAN:
				return new VulkanFence(DeviceObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
