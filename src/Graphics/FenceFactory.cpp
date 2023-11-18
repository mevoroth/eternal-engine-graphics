#include "Graphics/FenceFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullFence.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Vulkan/VulkanFence.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/FenceFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		Fence* CreateFence(_In_ Device& InDevice)
		{
			switch (InDevice.GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullFence();

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12Fence(InDevice);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanFence(InDevice);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateFencePrivate(InDevice);
#endif
				break;
			}

			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
