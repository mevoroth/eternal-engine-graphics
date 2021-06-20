#ifndef _HEAP_FACTORY_HPP_
#define _HEAP_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum HeapType;
		class Heap;
		class Device;

		Heap* CreateHeap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
	}
}

#endif
