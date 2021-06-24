#include "Graphics/CommandAllocatorFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandAllocator* CreateCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue)
		{
			switch (InDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12CommandAllocator(InDevice, InCommandQueue);
#endif
			case DeviceType::VULKAN:
				return new VulkanCommandAllocator(InDevice, InCommandQueue);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
