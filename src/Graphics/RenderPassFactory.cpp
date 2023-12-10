#include "Graphics/RenderPassFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullRenderPass.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "d3d12/D3D12RenderPass.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/RenderPassFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		RenderPass* CreateRenderPass(_In_ GraphicsContext& InContext, _In_ const RenderPassCreateInformation& InRenderPassCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullRenderPass(InRenderPassCreateInformation);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12RenderPass(InRenderPassCreateInformation);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanRenderPass(InContext, InRenderPassCreateInformation);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateRenderPassPrivate(InContext, InRenderPassCreateInformation);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyRenderPass(_Inout_ RenderPass*& InOutRenderPass)
		{
			delete InOutRenderPass;
			InOutRenderPass = nullptr;
		}
	}
}
