#include "Graphics/RootSignatureFactory.hpp"

#include "NextGenGraphics/Context.hpp"
#include "NextGenGraphics/Types/DeviceType.hpp"
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
			case DeviceType::D3D12:
				return new D3D12RootSignature(DeviceObj);
#endif

			case DeviceType::VULKAN:
				return new VulkanRootSignature(DeviceObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12RootSignature(DeviceObj, Resources, ResourcesCount, RootSignatureAccessObj);
#endif

			case DeviceType::VULKAN:
				return new VulkanRootSignature(DeviceObj, Resources, ResourcesCount, RootSignatureAccessObj);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		RootSignature* CreateRootSignature(_In_ GraphicsContext& Context)
		{
			return CreateRootSignature(Context.GetDevice());
		}

		RootSignature* CreateRootSignature(_In_ GraphicsContext& Context, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
		{
			return CreateRootSignature(Context.GetDevice(), Resources, ResourcesCount, RootSignatureAccessObj);
		}
	}
}
