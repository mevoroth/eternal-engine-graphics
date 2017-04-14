#include "NextGenGraphics/Resource.hpp"

#include "Graphics/Heap.hpp"

using namespace Eternal::Graphics;

Resource::Resource(_In_ Heap& HeapObj, _In_ uint64_t Width, _In_ uint64_t Height, _In_ uint64_t Depth, _In_ uint64_t MipCount)
	: _Heap(HeapObj)
	, _Width(Width)
	, _Height(Height)
	, _Depth(Depth)
	, _MipCount(MipCount)
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

uint64_t Resource::GetSize() const
{
	return _Width * _Height * _Depth;
}
