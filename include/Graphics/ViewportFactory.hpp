#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Viewport;

		Viewport* CreateViewport(_In_ GraphicsContext& InContext, _In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight);
		Viewport* CreateInvertedViewport(_In_ GraphicsContext& InContext, _In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight);
	}
}
