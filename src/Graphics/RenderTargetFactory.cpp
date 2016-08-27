#include "Graphics/RenderTargetFactory.hpp"

#include "d3d11/D3D11RenderTarget.hpp"

namespace Eternal
{
	namespace Graphics
	{
		RenderTarget* CreateRenderTarget(_In_ int Width, _In_ int Height, _In_ const Format& FormatObj)
		{
			return new D3D11RenderTarget(FormatObj, Width, Height);
		}
	}
}
