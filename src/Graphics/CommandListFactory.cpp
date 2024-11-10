#include "Graphics/CommandListFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/CommandListFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		CommandList* CreateCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
		{
			switch (InDevice.GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12CommandList(InDevice, InCommandAllocator);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanCommandList(InDevice, InCommandAllocator);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateCommandListPrivate(InDevice, InCommandAllocator);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
