#include "d3d11/D3D11Renderer.hpp"

#include <d3d11.h>
#include <cassert>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"

using namespace Eternal::Graphics;

D3D11Renderer::D3D11Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
{
	assert(mode != SOFTWARE); // NOT IMPLEMENTED YET

	HRESULT hr;

	hr = _CreateDevice();
	if (hr != S_OK)
	{
		assert(false);
		// ERROR
	}

	hr = _CreateSwapChain();
	if (hr != S_OK)
	{
		assert(false);
		// ERROR
	}

	// Get Back Buffer
	ID3D11Texture2D* BackBufferTex = 0;
	_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);
	_SetBackBuffer(new D3D11RenderTarget(BackBufferTex));
}

HRESULT D3D11Renderer::_CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL out;
	ID3D11DeviceContext* DeviceContext = nullptr;

	HRESULT hr = D3D11CreateDevice(
		0,
		GetRenderMode() == SOFTWARE ? D3D_DRIVER_TYPE_SOFTWARE : D3D_DRIVER_TYPE_HARDWARE,
		0, // SOFTWARE NOT IMPLEMENTED
		0
#ifdef ETERNAL_DEBUG
		//| D3D11_CREATE_DEVICE_DEBUG
		//| D3D11_CREATE_DEVICE_DEBUGGABLE
#endif
#ifdef ETERNAL_D3D_DEBUG
		| D3D11_CREATE_DEVICE_SINGLETHREADED
#endif
		,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&_Device,
		&out,
		&DeviceContext
	);

	if (out == 0)
	{
		// ERROR
		printf("ERROR\n");
		assert(false);
	}

	if (hr != S_OK)
	{
		// ERROR
		DWORD err = GetLastError();
		printf("ERROR %d:%x\n", err, err);
		assert(false);
	}

	_MainContext = new D3D11Context(DeviceContext);

	return hr;
}

HRESULT D3D11Renderer::_CreateSwapChain()
{
	IDXGIDevice* DxgiDevice;
	HRESULT hr = _Device->QueryInterface<IDXGIDevice>(&DxgiDevice);

	IDXGIAdapter* DxgiAdapter;
	hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&DxgiAdapter);

	IDXGIFactory* DxgiFactory;
	hr = DxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&DxgiFactory);


	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 60Hz
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;


	UINT Quality = 0;

	hr = _Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		(UINT)((GetAntiAliasing() & 0xffff0000) >> 16),
		&Quality
	);

	if ((GetAntiAliasing() & 0xffff) < Quality)
	{
		SwapChainDesc.SampleDesc.Count = ((GetAntiAliasing() & 0xffff0000) >> 16);
		SwapChainDesc.SampleDesc.Quality = (GetAntiAliasing() & 0xffff);
	}
	else
	{
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
	}

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = static_cast<D3D11Device*>(Device::Get())->GetWindow();
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = DxgiFactory->CreateSwapChain(
		_Device,
		&SwapChainDesc,
		&_SwapChain
	);

	if (hr != S_OK)
	{
		assert(false);
		// ERROR
		//char str[256];
		//DWORD err = GetLastError();
		//sprintf_s(str, "ERROR: %d:%x\n", err, err);
		//OutputDebugString(str);
	}

	return hr;
}

ID3D11Device* D3D11Renderer::GetDevice()
{
	assert(_Device);
	return _Device;
}

D3D11Context* D3D11Renderer::GetMainContext()
{
	assert(_MainContext);
	return _MainContext;
}

void D3D11Renderer::Flush()
{
	_SwapChain->Present(0, 0);
}

D3D11Context* D3D11Renderer::CreateDeferredContext()
{
	ID3D11DeviceContext* DeviceContext;
	_Device->CreateDeferredContext(0, &DeviceContext);
	return new D3D11Context(DeviceContext);
}
