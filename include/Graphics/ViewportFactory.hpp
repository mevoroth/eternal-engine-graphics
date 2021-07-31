#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Viewport;

		Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX = 0, _In_ int32_t InY = 0);
		Viewport* CreateInvertedViewport(_In_ GraphicsContext& InContext, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ int32_t InX = 0, _In_ int32_t InY = 0);
		void DestroyViewport(_Inout_ Viewport*& InOutViewport);
	}
}
