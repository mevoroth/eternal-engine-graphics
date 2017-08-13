#ifndef _COMMAND_ALLOCATOR_FACTORY_HPP_
#define _COMMAND_ALLOCATOR_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum CommandListType;
		class Device;
		class CommandAllocator;

		CommandAllocator* CreateCommandAllocator(_In_ Device& DeviceObj, _In_ const CommandListType& Type);
	}
}

#endif
