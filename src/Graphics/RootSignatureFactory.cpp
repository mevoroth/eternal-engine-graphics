#include "Graphics/RootSignatureFactory.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "d3d12/D3D12RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RootSignature* CreateRootSignature(_In_ Device& DeviceObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12RootSignature(DeviceObj);
#endif

			case VULKAN:
				return new VulkanRootSignature(DeviceObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
