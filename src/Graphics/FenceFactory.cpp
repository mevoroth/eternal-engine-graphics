#include "Graphics/FenceFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Vulkan/VulkanFence.hpp"

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
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
