#ifndef _VIEWPORT_FACTORY_HPP_
#define _VIEWPORT_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;

		Viewport* CreateViewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height);
	}
}

#endif
