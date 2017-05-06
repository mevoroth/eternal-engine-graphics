#include "Graphics/ResourceFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "Vulkan/VulkanResource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const ResourceType& Type)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12Resource(DeviceObj, HeapObj);
#endif

			case VULKAN:
				return new VulkanResource(DeviceObj, HeapObj, Size, Type);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
