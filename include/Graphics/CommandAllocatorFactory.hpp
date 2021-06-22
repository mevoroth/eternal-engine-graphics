#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class CommandQueue;

		CommandAllocator* CreateCommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue);
	}
}
