#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;

		Fence* CreateFence(_In_ Device& InDevice);
	}
}
