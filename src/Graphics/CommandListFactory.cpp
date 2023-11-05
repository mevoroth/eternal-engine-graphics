#include "Graphics/CommandListFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "Vulkan/VulkanCommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandList* CreateCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
		{
			switch (InDevice.GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12CommandList(InDevice, InCommandAllocator);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanCommandList(InDevice, InCommandAllocator);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
