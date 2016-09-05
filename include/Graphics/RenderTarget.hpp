#ifndef _RENDER_TARGET_HPP_
#define _RENDER_TARGET_HPP_

#include "Clearable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Resource;

		class RenderTarget : public Clearable
		{
		public:
			virtual Resource* GetAsResource() = 0;
		};
	}
}

#endif
