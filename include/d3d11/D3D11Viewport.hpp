#ifndef _D3D11_VIEWPORT_HPP_
#define _D3D11_VIEWPORT_HPP_

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Viewport : public Viewport
		{
		public:
			D3D11Viewport(_In_ int x, _In_ int y, _In_ int width, _In_ int height);
			virtual void Apply(Context* DrawContext) /*override*/;
		};
	}
}

#endif
