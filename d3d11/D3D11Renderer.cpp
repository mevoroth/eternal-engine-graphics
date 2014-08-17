#include "D3D11Renderer.hpp"

#include <cassert>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d11/D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Renderer* D3D11Renderer::_inst = 0;

D3D11Renderer::D3D11Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
	, _device(0)
{
	assert(mode != SOFTWARE); // NOT IMPLEMENTED YET

	assert(!_inst);
	_inst = this;

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
		D3D11_CREATE_DEVICE_SINGLETHREADED
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

	//for (UINT sampleCount = 1; sampleCount <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; sampleCount++)
	//{
	//	UINT maxQualityLevel;
	//	hr = _device->CheckMultisampleQualityLevels(
	//		DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &maxQualityLevel);

	//	if (maxQualityLevel > 0)
	//	{
	//		maxQualityLevel--;
	//	}

	//	if (hr != S_OK)
	//	{
	//		printf("CheckMultisampleQualityLevels failed.\n");
	//	}

	//	if (maxQualityLevel > 0)
	//	{
	//		char str[256];
	//		sprintf_s(str, "MSAA %dX supported with %d quality levels.\n", sampleCount, maxQualityLevel);
	//		OutputDebugString(str);
	//	}
	//}

	if (hr != S_OK)
	{
		// ERROR
	}

	IDXGIDevice* dxgiDevice;
	hr = _device->QueryInterface<IDXGIDevice>(&dxgiDevice);

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
	swapChainDesc.OutputWindow = D3D11DeviceType::get()->getWindow();
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
}

D3D11Renderer* D3D11Renderer::get()
{
	assert(_inst);
	return _inst;
}

ID3D11Device* D3D11Renderer::getDevice()
{
	assert(!_device);
	return _device;
}
