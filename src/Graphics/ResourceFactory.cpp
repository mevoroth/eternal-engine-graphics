#include "Graphics/ResourceFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "Vulkan/VulkanResource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const BufferType& Type)
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

		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& State)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				ETERNAL_ASSERT(false);
				return new D3D12Resource(DeviceObj, HeapObj);
#endif

			case VULKAN:
				return new VulkanResource(DeviceObj, HeapObj, Dimension, FormatObj, Type, Width, Height, Depth, MipCount, State);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
