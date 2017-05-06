#ifndef _HEAP_FACTORY_HPP_
#define _HEAP_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Heap;
		class Device;

		Heap* CreateHeap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
	}
}

#endif
