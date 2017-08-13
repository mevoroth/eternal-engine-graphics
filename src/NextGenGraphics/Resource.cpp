#include "NextGenGraphics/Resource.hpp"

#include "Graphics/Heap.hpp"

using namespace Eternal::Graphics;

Resource::Resource(_In_ Heap& HeapObj, _In_ const Format& ImageFormat, _In_ uint64_t Width, _In_ uint64_t Height, _In_ uint64_t Depth, _In_ uint64_t MipCount)
	: _Heap(HeapObj)
{
	_ResourceDescription.Width			= Width;
	_ResourceDescription.Height			= Height;
	_ResourceDescription.Depth			= Depth;
	_ResourceDescription.MipCount		= MipCount;
	_ResourceDescription.ImageFormat	= ImageFormat;
}

Resource::Resource(_In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size)
	: _Heap(HeapObj)
{
	_ResourceDescription.Stride		= Stride;
	_ResourceDescription.BufferSize = Size;
}

Resource::Resource()
	: _Heap(*(Heap*)nullptr)
{
}

Resource::~Resource()
{
	if (&_Heap != nullptr)
	{
		_Heap.Push(GetHeapSlot());
		SetHeapSlot(0xFFFFFFFFFFFFFFFF);
	}
}
