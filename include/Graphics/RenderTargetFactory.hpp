#ifndef _RENDER_TARGET_FACTORY_HPP_
#define _RENDER_TARGET_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		class RenderTarget;

		RenderTarget* CreateRenderTarget(_In_ int Width, _In_ int Height, _In_ const Format& FormatObj);
		RenderTarget* CreateDepthStencilRenderTarget(_In_ int Width, _In_ int Height);
	}
}

#endif
