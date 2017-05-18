#include "NextGenGraphics/SamplerFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "Vulkan/VulkanSampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Sampler* CreateSampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12Sampler(DeviceObj, DescriptorHeapObj, MINLinear, MAGLinear, MIPLinear, Comparison, U, V, W);
#endif

			case VULKAN:
			{
				if (!Comparison)
					return new VulkanSampler(DeviceObj, MINLinear, MAGLinear, MIPLinear, U, V, W);
			}
			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}

		Sampler* CreateStaticSampler(_In_ Device& DeviceObj, _In_ uint32_t Register, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12Sampler(DeviceObj, Register, MINLinear, MAGLinear, MIPLinear, Comparison, U, V, W);
#endif

			case VULKAN:
			{
				ETERNAL_ASSERT(false);
				if (!Comparison)
					return new VulkanSampler(DeviceObj, MINLinear, MAGLinear, MIPLinear, U, V, W);
			}

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
