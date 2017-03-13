#ifndef _COMMAND_LIST_HPP_
#define _COMMAND_LIST_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;

		class CommandList
		{
		public:
			virtual void SetViewport(_In_ Viewport& ViewportObj) = 0;
			virtual void SetScissorRectangle(_In_ Viewport& ViewportObj) = 0;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) = 0;
			virtual void End() = 0;
		};
	}
}

#endif