#ifndef _COMMAND_LIST_FACTORY_HPP_
#define _COMMAND_LIST_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum class CommandType;
		class Device;
		class CommandList;

		CommandList* CreateCommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type);
	}
}

#endif
