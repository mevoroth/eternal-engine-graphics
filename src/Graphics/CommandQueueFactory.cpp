#include "Graphics/CommandQueueFactory.hpp"

#include "Log/Log.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullCommandQueue.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/CommandQueueFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		CommandQueue* CreateCommandQueue(_In_ Device& InDevice, _In_ const CommandType& InType)
		{
			LogWrite(LogInfo, LogGraphics, "[Graphics::CreateCommandQueue]Creating Command Queue");

			switch (InDevice.GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullCommandQueue(InType);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12CommandQueue(InDevice, InType);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanCommandQueue(InDevice, InType);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateCommandQueuePrivate(InDevice, InType);
#endif
				break;
			}

			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
