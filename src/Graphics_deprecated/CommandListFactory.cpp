#include "Graphics_deprecated/CommandListFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12CommandList.hpp"
#include "Vulkan_deprecated/VulkanCommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandList* CreateCommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12CommandList(DeviceObj, Type);
#endif

			case DeviceType::VULKAN:
				return new VulkanCommandList(DeviceObj, Type);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
