#include "Graphics/SamplerFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Null/NullSampler.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "Vulkan/VulkanSampler.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/SamplerFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		Sampler* CreateSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullSampler(InSamplerCreateInformation);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12Sampler(InContext, InSamplerCreateInformation);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanSampler(InContext, InSamplerCreateInformation);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateSamplerPrivate(InContext, InSamplerCreateInformation);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroySampler(_Inout_ Sampler*& InOutSampler)
		{
			delete InOutSampler;
			InOutSampler = nullptr;
		}
	}
}
