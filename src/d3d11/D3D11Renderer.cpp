#include "d3d11/D3D11Renderer.hpp"

#include <cassert>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11RenderTarget.hpp"

using namespace Eternal::Graphics;

D3D11Renderer* D3D11Renderer::_inst = 0;

D3D11Renderer::D3D11Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
	, _device(0)
	, _deviceContext(0)
	, _swapChain(0)
	, _camera(0)
	, _renderTargets(0)
	, _renderTargetsCount(0)
{
	assert(mode != SOFTWARE); // NOT IMPLEMENTED YET

	assert(!_inst);
	_inst = this;

	HRESULT hr;

	hr = _CreateDevice();
	if (hr != S_OK)
	{
		// ERROR
	}

	hr = _CreateSwapChain();
	if (hr != S_OK)
	{
		// ERROR
	}

	// Get Back Buffer
	ID3D11Texture2D* backBufferTex = 0;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTex);
	_SetBackBuffer(new D3D11RenderTarget(backBufferTex));
}

HRESULT D3D11Renderer::_CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL out;

	HRESULT hr = D3D11CreateDevice(
		0,
		GetRenderMode() == SOFTWARE ? D3D_DRIVER_TYPE_SOFTWARE : D3D_DRIVER_TYPE_HARDWARE,
		0, // SOFTWARE NOT IMPLEMENTED
		0
#ifdef __DEBUG__
		| D3D11_CREATE_DEVICE_DEBUG
		| D3D11_CREATE_DEVICE_DEBUGGABLE
#endif
#ifdef __D3D11_SINGLETHREAD__
		| D3D11_CREATE_DEVICE_SINGLETHREADED
#endif
		,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&_device,
		&out,
		&_deviceContext
	);

	if (out == 0)
	{
		// ERROR
		printf("ERROR\n");
	}

	if (hr != S_OK)
	{
		// ERROR
		DWORD err = GetLastError();
		printf("ERROR %d:%x\n", err, err);
	}

	return hr;
}

HRESULT D3D11Renderer::_CreateSwapChain()
{
	IDXGIDevice* dxgiDevice;
	HRESULT hr = _device->QueryInterface<IDXGIDevice>(&dxgiDevice);

	IDXGIAdapter* dxgiAdapter;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);


	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 60Hz
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;


	UINT quality = 0;

	hr = _device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		(UINT)((GetAntiAliasing() & 0xffff0000) >> 16),
		&quality
	);

	if ((GetAntiAliasing() & 0xffff) < quality)
	{
		swapChainDesc.SampleDesc.Count = ((GetAntiAliasing() & 0xffff0000) >> 16);
		swapChainDesc.SampleDesc.Quality = (GetAntiAliasing() & 0xffff);
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = D3D11DeviceType::Get()->GetWindow();
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = dxgiFactory->CreateSwapChain(
		_device,
		&swapChainDesc,
		&_swapChain
	);

	if (hr != S_OK)
	{
		// ERROR
		char str[256];
		DWORD err = GetLastError();
		sprintf_s(str, "ERROR: %d:%x\n", err, err);
		OutputDebugString(str);
	}
	return hr;
}

D3D11Renderer* D3D11Renderer::Get()
{
	assert(_inst);
	return _inst;
}

ID3D11Device* D3D11Renderer::GetDevice()
{
	assert(_device);
	return _device;
}

ID3D11DeviceContext* D3D11Renderer::GetDeviceContext()
{
	assert(_deviceContext);
	return _deviceContext;
}

void D3D11Renderer::Flush()
{
	_swapChain->Present(0, 0);
}

void D3D11Renderer::AttachCamera(_In_ Camera<XMVECTOR, XMMATRIX>* camera)
{
	_camera = camera;
}

void D3D11Renderer::AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count)
{
	assert(renderTargets);
	assert(count > 0);
	_renderTargets = renderTargets;
	_renderTargetsCount = count;
}

void D3D11Renderer::ClearRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count)
{
	assert(renderTargets);
	assert(count > 0);
	for (int i = 0; i < count; ++i)
	{
		renderTargets[i]->Clear();
	}
}
