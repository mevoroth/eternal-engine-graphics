#include "d3d12/D3D12Device.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>

#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "Window/Window.hpp"

using namespace Eternal::Graphics;

#ifdef ETERNAL_DEBUG
#define ETERNAL_D3D12_DXGIFLAG	(0x1)
#else
#define ETERNAL_D3D12_DXGIFLAG	(0x0)
#endif

ID3D12Debug* D3D12Device::_Debug = nullptr;
IDXGIFactory4* D3D12Device::_DXGIFactory = nullptr;

void D3D12Device::Initialize()
{
	HRESULT hr = CreateDXGIFactory2(ETERNAL_D3D12_DXGIFLAG, __uuidof(IDXGIFactory4), (void**)&_DXGIFactory);
	ETERNAL_ASSERT(hr == S_OK);

#ifdef ETERNAL_DEBUG
	// Enable the D3D12 debug layer
	hr = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&_Debug);
	if (hr == S_OK)
	{
		_Debug->EnableDebugLayer();
	}
#endif

}

IDXGIFactory4* D3D12Device::GetDXGIFactory()
{
	return _DXGIFactory;
}

D3D12Device::D3D12Device(_In_ uint32_t DeviceIndex)
{
	ETERNAL_ASSERT(_DXGIFactory);
	HRESULT hr = _DXGIFactory->EnumAdapters1(DeviceIndex, &_DXGIAdapter);
	
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		// No GPU at this index
		return;
	}

	hr = D3D12CreateDevice(_DXGIAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)&_Device);
	ETERNAL_ASSERT(hr == S_OK);
	ETERNAL_ASSERT(_Device);

#ifdef ETERNAL_DEBUG
	DXGI_ADAPTER_DESC DXGIAdapterDesc;
	hr = _DXGIAdapter->GetDesc(&DXGIAdapterDesc);
	ETERNAL_ASSERT(hr == S_OK); // Break here for debug info on device
#endif

	_DeviceMask = 1 << DeviceIndex;
}

uint32_t D3D12Device::GetDeviceMask() const
{
	ETERNAL_ASSERT(_DeviceMask != 0xFFFFFFFF);
	return _DeviceMask;
}
