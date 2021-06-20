#ifndef _COMMAND_LIST_FACTORY_HPP_
#define _COMMAND_LIST_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum CommandListType;
		class Device;
		class CommandList;

		CommandList* CreateCommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type);
	}
}

#endif
