#include "Graphics_deprecated/CommandQueueFactory.hpp"

#include "Log/Log.hpp"
#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12CommandQueue.hpp"
#include "Vulkan_deprecated/VulkanCommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandQueue* CreateCommandQueue(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
		{
			LogWrite(LogInfo, LogEngine, "[Graphics::CreateDevice]Creating Command Queue");

			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12CommandQueue(DeviceObj, Type);
#endif

			case DeviceType::VULKAN:
				return new VulkanCommandQueue(DeviceObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
			
		}
	}
}
