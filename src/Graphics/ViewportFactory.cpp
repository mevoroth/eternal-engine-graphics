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
		static Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX, _In_ int32_t InY, _In_ void* InViewportPlacementMemory)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
			{
				if (InViewportPlacementMemory)
					return new (InViewportPlacementMemory) Viewport(InX, InY, InWidth, InHeight);
				return new Viewport(InX, InY, InWidth, InHeight);
			}
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				if (InViewportPlacementMemory)
					return new (InViewportPlacementMemory) VulkanViewport(InX, InY, InWidth, InHeight, IsHeightInverted);
				return new VulkanViewport(InX, InY, InWidth, InHeight, IsHeightInverted);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
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
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return sizeof(Viewport);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
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
