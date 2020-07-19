#ifndef _FENCE_FACTORY_HPP_
#define _FENCE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;

		Fence* CreateFence(_In_ Device& DeviceObj);
	}
}

#endif
