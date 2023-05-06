#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Viewport;

		Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX = 0, _In_ int32_t InY = 0, _In_ void* InViewportPlacementMemory = nullptr);
		Viewport* CreateInvertedViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX = 0, _In_ int32_t InY = 0, _In_ void* InViewportPlacementMemory = nullptr);
		size_t GetViewportSize(_In_ GraphicsContext& InContext);
		void DestroyViewport(_Inout_ Viewport*& InOutViewport);
		void DestroyInPlaceViewport(_Inout_ Viewport*& InOutViewport);
	}
}
