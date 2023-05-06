#pragma once

#include "Graphics/CommandList.hpp"
#include "Graphics/CommandUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		CommandQueue* CreateCommandQueue(_In_ Device& InDevice, _In_ const CommandType& InType = CommandType::COMMAND_TYPE_GRAPHICS);
	}
}
