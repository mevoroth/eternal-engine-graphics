#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.h"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		CommandQueue* CreateCommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type = CommandType::COMMAND_TYPE_GRAPHIC);
	}
}
