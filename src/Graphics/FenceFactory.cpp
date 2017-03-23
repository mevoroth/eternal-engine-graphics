#include "Graphics/FenceFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Vulkan/VulkanFence.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Fence* CreateFence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount)
		{
			switch (DeviceObj.GetDeviceType())
			{
			case D3D12:
				return new D3D12Fence(DeviceObj, SimultaneousResourcesCount);

			case VULKAN:
				return new VulkanFence(DeviceObj, SimultaneousResourcesCount);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
