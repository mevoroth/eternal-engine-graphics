#include "Graphics/ContextFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "Vulkan/VulkanContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Context* CreateContext(_In_ Device& DeviceObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new Context(DeviceObj);
#endif
			case DeviceType::VULKAN:
				return new VulkanContext(DeviceObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
