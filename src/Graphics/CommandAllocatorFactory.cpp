#include "Graphics/CommandAllocatorFactory.hpp"

#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandAllocator* CreateCommandAllocator(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12CommandAllocator(DeviceObj, Type);
#endif
			case VULKAN:
				return new VulkanCommandAllocator(DeviceObj);

			default:
				break;
			}
		}
	}
}
