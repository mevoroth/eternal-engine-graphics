#include "Graphics/CommandQueueFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "VUlkan/VulkanCommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandQueue* CreateCommandQueue(_In_ Device& DeviceObj, _In_ uint32_t FrameCount)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef _WIN32_WINNT >= _WIN32_WINNT_WIN10
			case D3D12:
				return new D3D12CommandQueue(DeviceObj, FrameCount);
#endif

			case VULKAN:
				return new VulkanCommandQueue(DeviceObj, FrameCount);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
			
		}
	}
}
