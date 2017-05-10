#include "Graphics/DescriptorHeapFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "Vulkan/VulkanDescriptorHeap.hpp"

namespace Eternal
{
	namespace Graphics
	{
		DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12DescriptorHeap(DeviceObj, Space, HeapTypeObj, ResourcesCount);
#endif

			case VULKAN:
				return new VulkanDescriptorHeap(DeviceObj, Space, HeapTypeObj, ResourcesCount, RootSignatureAccessObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
