#include "Graphics/Heap.hpp"

using namespace Eternal::Graphics;

Heap::Heap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount)
	: _Device(DeviceObj)
	, _HeapType(HeapTypeObj)
	, _ResourcesCount(ResourcesCount)
{
	ETERNAL_ASSERT(_ResourcesCount > 0);

	_ResourcesPool.resize(ResourcesCount);
	for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
	{
		_ResourcesPool[ResourceIndex] = ResourcesCount - ResourceIndex - 1u;
	}
}

uint64_t Heap::Pop()
{
	ETERNAL_ASSERT(_Size > 0);
	uint64_t Slot = _ResourcesPool.back();
	_ResourcesPool.pop_back();
	return Slot * _Size;
}

void Heap::Push(_In_ uint64_t Offset)
{
	_ResourcesPool.push_back(Offset / _Size);
}
