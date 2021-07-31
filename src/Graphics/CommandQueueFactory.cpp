#include "Graphics/CommandQueueFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandQueue* CreateCommandQueue(_In_ Device& InDevice, _In_ const CommandType& InType)
		{
			LogWrite(LogInfo, LogGraphics, "[Graphics::CreateDevice]Creating Command Queue");

			switch (InDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12CommandQueue(InDevice, InType);
#endif

			case DeviceType::VULKAN:
				return new VulkanCommandQueue(InDevice, InType);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
			
		}
	}
}
