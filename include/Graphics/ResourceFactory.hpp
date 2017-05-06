#ifndef _RESOURCE_FACTORY_HPP_
#define _RESOURCE_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum ResourceType;

		class Device;
		class Heap;
		class Resource;

		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Size, _In_ const ResourceType& Type);
	}
}

#endif
