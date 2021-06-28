#include "Graphics/ResourceFactory.hpp"

#include "Graphics/Resource.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "Vulkan/VulkanResource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename ResourceCreateInformationType>
		static Resource* CreateResource(_In_ const ResourceCreateInformationType& InResourceCreateInformation)
		{
			switch (InResourceCreateInformation.GfxDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Resource(InResourceCreateInformation);
#endif

			case DeviceType::VULKAN:
				return new VulkanResource(InResourceCreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
				break;
			}
		}

		Resource* CreateTexture(_In_ const TextureResourceCreateInformation& InResourceCreateInformation)
		{
			return CreateResource(InResourceCreateInformation);
		}

		Resource* CreateBuffer(_In_ const BufferResourceCreateInformation& InResourceCreateInformation)
		{
			return CreateResource(InResourceCreateInformation);
		}
	}
}
