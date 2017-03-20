#ifndef _FENCE_FACTORY_HPP_
#define _FENCE_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;

		Fence* CreateFence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount = 1u);
	}
}

#endif
