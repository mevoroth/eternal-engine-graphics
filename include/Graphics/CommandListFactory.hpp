#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandList;
		class CommandAllocator;

		CommandList* CreateCommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
	}
}
