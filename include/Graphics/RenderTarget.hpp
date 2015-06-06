#ifndef _RENDER_TARGET_HPP_
#define _RENDER_TARGET_HPP_

#include "Texture.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class RenderTarget
		{
		public:
			virtual void Clear(Context* ContextObj) = 0;
		};
	}
}

#endif
