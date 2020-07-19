#include "Graphics/CommandListFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "Vulkan/VulkanCommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandList* CreateCommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
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
