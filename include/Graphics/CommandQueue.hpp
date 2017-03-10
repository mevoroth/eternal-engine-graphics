#ifndef _COMMAND_QUEUE_HPP_
#define _COMMAND_QUEUE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class CommandQueue
		{
		public:
			virtual void Reset(_In_ uint32_t FrameIndex) = 0;
		};
	}
}

#endif
