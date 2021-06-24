#include "NextGenGraphics/SamplerFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12_deprecated/D3D12Sampler.hpp"
#include "Vulkan_deprecated/VulkanSampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Sampler* CreateSampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Sampler(DeviceObj, DescriptorHeapObj, MINLinear, MAGLinear, MIPLinear, Comparison, U, V, W);
#endif

			case DeviceType::VULKAN:
			{
				if (!Comparison)
					return new VulkanSampler(DeviceObj, MINLinear, MAGLinear, MIPLinear, U, V, W);
			}
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		Sampler* CreateStaticSampler(_In_ Device& DeviceObj, _In_ uint32_t Register, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Sampler(DeviceObj, Register, MINLinear, MAGLinear, MIPLinear, Comparison, U, V, W);
#endif

			case DeviceType::VULKAN:
			{
				//ETERNAL_BREAK(); // TODO: FIX THIS
				if (!Comparison)
					return new VulkanSampler(DeviceObj, MINLinear, MAGLinear, MIPLinear, U, V, W);
			}

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
