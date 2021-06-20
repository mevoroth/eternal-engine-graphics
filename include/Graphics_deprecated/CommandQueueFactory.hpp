#ifndef _COMMAND_QUEUE_FACTORY_HPP_
#define _COMMAND_QUEUE_FACTORY_HPP_

#include "Graphics_deprecated/CommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		CommandQueue* CreateCommandQueue(_In_ Device& DeviceObj, _In_ const CommandListType& Type = COMMAND_LIST_TYPE_GRAPHIC);
	}
}

#endif
