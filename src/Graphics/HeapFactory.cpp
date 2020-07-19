#include "Graphics/HeapFactory.hpp"

#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Heap.hpp"
#include "Vulkan/VulkanHeap.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Heap* CreateHeap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Heap(DeviceObj, HeapTypeObj, ResourcesCount, InVRAM, VisibleFromCPU, Coherent, Cached);
#endif

			case DeviceType::VULKAN:
				return new VulkanHeap(DeviceObj, HeapTypeObj, ResourcesCount, InVRAM, VisibleFromCPU, Coherent, Cached);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
