#include "Graphics/FenceFactory.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Fence.hpp"
#include "Vulkan/VulkanFence.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Fence* CreateFence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef _WIN32_WINNT >= _WIN32_WINNT_WIN10
			case D3D12:
				return new D3D12Fence(DeviceObj, SimultaneousResourcesCount);
#endif

			case VULKAN:
				return new VulkanFence(DeviceObj, SimultaneousResourcesCount);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
