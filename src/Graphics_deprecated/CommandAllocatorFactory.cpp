#include "Graphics_deprecated/CommandAllocatorFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12CommandAllocator.hpp"
#include "Vulkan_deprecated/VulkanCommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandAllocator* CreateCommandAllocator(_In_ Device& DeviceObj, _In_ const CommandType& Type)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12CommandAllocator(DeviceObj, Type);
#endif
			case DeviceType::VULKAN:
				return new VulkanCommandAllocator(DeviceObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
