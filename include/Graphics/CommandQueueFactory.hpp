#ifndef _COMMAND_QUEUE_FACTORY_HPP_
#define _COMMAND_QUEUE_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		CommandQueue* CreateCommandQueue(_In_ Device& DeviceObj, _In_ uint32_t FrameCount);
	}
}

#endif
