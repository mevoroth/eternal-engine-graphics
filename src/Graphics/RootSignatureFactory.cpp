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
				return nullptr;
#endif

			case VULKAN:
				return new VulkanRootSignature(DeviceObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}

//		RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
//		{
//			switch (DeviceObj.GetDeviceType())
//			{
//#ifdef ETERNAL_ENABLE_D3D12
//			case D3D12:
//				return new D3D12RootSignature(DeviceObj, StaticSamplers, StaticSamplersCount, DescriptorHeaps, DescriptorHeapsCount, RootSignatureAccessObj);
//#endif
//
//			case VULKAN:
//				return new VulkanRootSignature(DeviceObj, StaticSamplers, StaticSamplersCount, DescriptorHeaps, DescriptorHeapsCount, RootSignatureAccessObj);
//
//			default:
//				ETERNAL_ASSERT(false);
//				return nullptr;
//			}
//		}

		RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12RootSignature(DeviceObj, Resources, ResourcesCount, RootSignatureAccessObj);
#endif

			case VULKAN:
				return new VulkanRootSignature(DeviceObj, Resources, ResourcesCount, RootSignatureAccessObj);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
