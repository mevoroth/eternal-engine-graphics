#ifndef _D3D11_VIEWPORT_HPP_
#define _D3D11_VIEWPORT_HPP_

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class D3D11Viewport : public Viewport
		{
		public:
			D3D11Viewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height);
			virtual void Apply(Context* DrawContext) /*override*/;
		};
	}
}

#endif
