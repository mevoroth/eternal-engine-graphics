#ifndef _FENCE_HPP_
#define _FENCE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		class Fence
		{
		public:
			virtual ~Fence() {}

			virtual void Signal(_In_ CommandQueue& CommandQueueObj) = 0;
			virtual void Wait(_In_ Device& DeviceObj) = 0;
			virtual void Reset(_In_ Device& DeviceObj) = 0;
		};
	}
}

#endif
