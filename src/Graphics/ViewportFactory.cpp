#include "Graphics/ViewportFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "Vulkan/VulkanViewport.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/ViewportFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		template<bool IsHeightInverted>
		static Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY, _In_ void* InViewportPlacementMemory)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
#endif
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
			{
				if (InViewportPlacementMemory)
					return new (InViewportPlacementMemory) Viewport(InX, InY, InWidth, InHeight);
				return new Viewport(InX, InY, InWidth, InHeight);
			}
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				if (InViewportPlacementMemory)
					return new (InViewportPlacementMemory) VulkanViewport(InX, InY, InWidth, InHeight, IsHeightInverted);
				return new VulkanViewport(InX, InY, InWidth, InHeight, IsHeightInverted);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateViewportPrivate(InContext, InWidth, InHeight, InX, InY, InViewportPlacementMemory);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY, _In_ void* InViewportPlacementMemory)
		{
			return CreateViewport<false>(InContext, InWidth, InHeight, InX, InY, InViewportPlacementMemory);
		}

		Viewport* CreateInvertedViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY, _In_ void* InViewportPlacementMemory)
		{
			return CreateViewport<true>(InContext, InWidth, InHeight, InX, InY, InViewportPlacementMemory);
		}

		size_t GetViewportSize(_In_ GraphicsContext& InContext)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
#endif
			case DeviceType::DEVICE_TYPE_NULL:
				return sizeof(Viewport);
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return sizeof(VulkanViewport);
#endif
			default:
				ETERNAL_BREAK();
				return 0ull;
			}
		}

		void DestroyViewport(_Inout_ Viewport*& InOutViewport)
		{
			delete InOutViewport;
			InOutViewport = nullptr;
		}

		void DestroyInPlaceViewport(_Inout_ Viewport*& InOutViewport)
		{
			InOutViewport->~Viewport();
			InOutViewport = nullptr;
		}
	}
}
