#include "Graphics_deprecated/ViewportFactory.hpp"

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Viewport* CreateViewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height)
		{
			return new Viewport(X, Y, Width, Height);
		}
	}
}
