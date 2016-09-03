#include "Graphics/ViewportFactory.hpp"

#include "d3d11/D3D11Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Viewport* CreateViewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height)
		{
			return new D3D11Viewport(X, Y, Width, Height);
		}
	}
}
