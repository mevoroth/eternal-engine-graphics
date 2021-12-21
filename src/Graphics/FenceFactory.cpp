#include "Graphics/FenceFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Vulkan/VulkanFence.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Fence* CreateFence(_In_ Device& InDevice)
		{
			switch (InDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Fence(InDevice);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanFence(InDevice);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
