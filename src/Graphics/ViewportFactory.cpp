#include "Graphics/ViewportFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "Vulkan/VulkanViewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<bool IsHeightInverted>
		static Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new Viewport(InX, InY, InWidth, InHeight);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanViewport(InX, InY, InWidth, InHeight, IsHeightInverted);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
				break;
			}
		}

		Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY)
		{
			return CreateViewport<false>(InContext, InWidth, InHeight, InX, InY);
		}

		Viewport* CreateInvertedViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY)
		{
			return CreateViewport<true>(InContext, InWidth, InHeight, InX, InY);
		}

		void DestroyViewport(_Inout_ Viewport*& InOutViewport)
		{
			delete InOutViewport;
			InOutViewport = nullptr;
		}
	}
}
