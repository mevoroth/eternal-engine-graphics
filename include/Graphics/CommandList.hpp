#pragma once

#include "Graphics/CommandUtils.h"

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		class Device;

		class CommandList
		{
		public:
			CommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type);
			virtual ~CommandList();

		private:
			CommandType _CommandType = CommandType::COMMAND_TYPE_GRAPHIC;
		};
	}
}
