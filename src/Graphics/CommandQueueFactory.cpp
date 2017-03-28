#include "Graphics/CommandQueueFactory.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
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
#ifdef ETERNAL_ENABLE_D3D12
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
