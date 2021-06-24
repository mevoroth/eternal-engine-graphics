#include "Graphics_deprecated/DescriptorHeapFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"
#include "Vulkan_deprecated/VulkanDescriptorHeap.hpp"

namespace Eternal
{
	namespace Graphics
	{
//		DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
//		{
//			switch (DeviceObj.GetDeviceType())
//			{
//#ifdef ETERNAL_ENABLE_D3D12
//			case D3D12:
//				return new D3D12DescriptorHeap(DeviceObj, Space, Register, HeapTypeObj, ResourcesCount);
//#endif
//
//			case VULKAN:
//				return new VulkanDescriptorHeap(DeviceObj, Space, Register, HeapTypeObj, ResourcesCount, RootSignatureAccessObj);
//
//			default:
//				ETERNAL_BREAK();
//				return nullptr;
//			}
//		}

		DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12DescriptorHeap(DeviceObj, Resources, ResourcesCount);
#endif

			case DeviceType::VULKAN:
				return new VulkanDescriptorHeap(DeviceObj, Resources, ResourcesCount);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
