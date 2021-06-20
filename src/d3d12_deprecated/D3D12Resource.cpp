#include "d3d12/D3D12Resource.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "d3d12/D3D12Heap.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Format.hpp"

using namespace Eternal::Graphics;

D3D12Resource::D3D12Resource(Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& State)
	: Resource(HeapObj, FormatObj, Width, Height, Depth, MipCount)
	, _DebugCurrentTransition(TRANSITION_UNDEFINED)
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Dimension			= (D3D12_RESOURCE_DIMENSION)(Dimension + D3D12_RESOURCE_DIMENSION_TEXTURE1D);
	ResourceDesc.Alignment			= 0;
	ResourceDesc.Width				= Width;
	ResourceDesc.Height				= Height;
	ResourceDesc.DepthOrArraySize	= Depth;
	ResourceDesc.MipLevels			= MipCount;
	ResourceDesc.Format				= D3D12_FORMATS[FormatObj];
	ResourceDesc.SampleDesc.Count	= 1;
	ResourceDesc.SampleDesc.Quality	= 0;
	ResourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;
	ResourceDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;
	if (Type & TEXTURE_RENDER_TARGET)
		ResourceDesc.Flags			|= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	if (Type & TEXTURE_DEPTH_STENCIL)
		ResourceDesc.Flags			|= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	if (Type & TEXTURE_WRITE)
		ResourceDesc.Flags			|= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	
	D3D12_RESOURCE_ALLOCATION_INFO ResourceAllocationInfo = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetResourceAllocationInfo(DeviceObj.GetDeviceMask(), 1, &ResourceDesc);
	if (!HeapObj.IsInitialized())
		HeapObj.Initialize(ResourceAllocationInfo.SizeInBytes);
	SetHeapSlot(HeapObj.Pop());

	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreatePlacedResource(
		static_cast<D3D12Heap&>(HeapObj).GetD3D12Heap(),
		GetHeapSlot(),
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&_Resource
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12Resource::D3D12Resource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type)
	: Resource(HeapObj, Stride, Size)
	, _DebugCurrentTransition(TRANSITION_UNDEFINED)
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Alignment			= 0;
	ResourceDesc.Width				= Size;
	ResourceDesc.Height				= 1;
	ResourceDesc.DepthOrArraySize	= 1;
	ResourceDesc.MipLevels			= 1;
	ResourceDesc.Format				= DXGI_FORMAT_UNKNOWN;
	ResourceDesc.SampleDesc.Count	= 1;
	ResourceDesc.SampleDesc.Quality	= 0;
	ResourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ResourceDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;
	
	D3D12_RESOURCE_ALLOCATION_INFO ResourceAllocationInfo = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetResourceAllocationInfo(DeviceObj.GetDeviceMask(), 1, &ResourceDesc);
	if (!HeapObj.IsInitialized())
		HeapObj.Initialize(ResourceAllocationInfo.SizeInBytes);
	SetHeapSlot(HeapObj.Pop());

	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreatePlacedResource(
		static_cast<D3D12Heap&>(HeapObj).GetD3D12Heap(),
		GetHeapSlot(),
		&ResourceDesc,
		static_cast<D3D12Heap&>(HeapObj).GetD3D12HeapType() == D3D12_HEAP_TYPE_UPLOAD ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&_Resource
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12Resource::D3D12Resource(_In_ ID3D12Resource* BackBuffer)
	: Resource()
	, _Resource(BackBuffer)
	, _DebugCurrentTransition(TRANSITION_PRESENT)
{
}

D3D12Resource::~D3D12Resource()
{
	if (&GetHeap())
	{
		_Resource->Release();
		_Resource = nullptr;
	}
}

View* D3D12Resource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
{
	return new D3D12View(DeviceObj, DescriptorHeapObj, *this, ViewType, FormatObj);
}

View* D3D12Resource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset, _In_ uint64_t Size)
{
	ETERNAL_BREAK();
	return nullptr;
}

View* D3D12Resource::CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj)
{
	return new D3D12View(DeviceObj, DescriptorHeapObj, *this, FormatObj);
}

View* D3D12Resource::CreateDepthStencilView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj)
{
	return new D3D12View(DeviceObj, DescriptorHeapObj, *this, FormatObj);
}

void* D3D12Resource::Map(_In_ Device& DeviceObj)
{
	D3D12_RANGE Range;
	Range.Begin	= 0;
	Range.End	= 0;
	void* Data;
	HRESULT hr = _Resource->Map(0, &Range, &Data);
	ETERNAL_ASSERT(hr == S_OK);
	return Data;
}

void D3D12Resource::Unmap(_In_ Device& DeviceObj)
{
	_Resource->Unmap(0, nullptr);
}

void D3D12Resource::SetName(_In_ const wchar_t* Name)
{
#ifdef ETERNAL_DEBUG
	_Resource->SetName(Name);
#endif
}
