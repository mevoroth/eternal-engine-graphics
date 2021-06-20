#include "Graphics_deprecated/ResourceFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12Resource.hpp"
#include "Vulkan_deprecated/VulkanResource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const wchar_t* Name, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
			{
				Resource* ResourceObj = new D3D12Resource(DeviceObj, HeapObj, Stride, Size, Type);
				ResourceObj->SetName(Name);
				return ResourceObj;
			}
#endif

			case DeviceType::VULKAN:
				return new VulkanResource(DeviceObj, HeapObj, Stride, Size, Type);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const wchar_t* Name, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& InitialState)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Resource(DeviceObj, HeapObj, Dimension, FormatObj, Type, Width, Height, Depth, MipCount, InitialState);
#endif

			case DeviceType::VULKAN:
				return new VulkanResource(DeviceObj, HeapObj, Dimension, FormatObj, Type, Width, Height, Depth, MipCount, InitialState);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
