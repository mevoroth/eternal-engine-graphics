#include "NextGenGraphics/Resource.hpp"

#include "Graphics/Heap.hpp"

using namespace Eternal::Graphics;

Resource::Resource(_In_ Heap& HeapObj)
	: _Heap(HeapObj)
{
	if (&_Heap != nullptr)
		SetHeapSlot(_Heap.Pop());
}

Resource::~Resource()
{
	if (&_Heap != nullptr)
	{
		_Heap.Push(GetHeapSlot());
		SetHeapSlot(0xFFFFFFFFFFFFFFFF);
	}
}
