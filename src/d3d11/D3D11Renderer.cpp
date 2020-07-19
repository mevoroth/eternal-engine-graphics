#include "d3d11/D3D11Renderer.hpp"

#include <d3d11.h>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11Context.hpp"
#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"

using namespace Eternal::Graphics;

D3D11Renderer::D3D11Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
{
	ETERNAL_ASSERT(mode != SOFTWARE); // NOT IMPLEMENTED YET

	HRESULT hr;

	hr = _CreateDevice();
	if (hr != S_OK)
	{
		ETERNAL_BREAK();
		// ERROR
	}

	hr = _CreateSwapChain();
	if (hr != S_OK)
	{
		ETERNAL_BREAK();
		// ERROR
	}

	// Get Back Buffer
	ID3D11Texture2D* BackBufferTex = nullptr;
	_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);
	_SetBackBuffer(new D3D11RenderTarget(BackBufferTex));
}

D3D11Renderer::~D3D11Renderer()
{
	delete _MainContext;
	_MainContext = nullptr;

	//ID3D11Device* _Device = nullptr;
	//IDXGISwapChain* _SwapChain = nullptr;
	//ID3D11Debug* _Debug = nullptr;
	//ID3D11InfoQueue* _InfoQueue = nullptr;
}

int D3D11Renderer::_CreateDevice()
{
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL Out;
	ID3D11DeviceContext* DeviceContext = nullptr;

	HRESULT hr = D3D11CreateDevice(
		0,
		GetRenderMode() == SOFTWARE ? D3D_DRIVER_TYPE_SOFTWARE : D3D_DRIVER_TYPE_HARDWARE,
		0, // SOFTWARE NOT IMPLEMENTED
		0
#ifdef ETERNAL_DEBUG
		| D3D11_CREATE_DEVICE_DEBUG
		//| D3D11_CREATE_DEVICE_DEBUGGABLE
#endif
#ifdef ETERNAL_D3D_DEBUG
		//| D3D11_CREATE_DEVICE_SINGLETHREADED
#endif
		,
		&FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&_Device,
		&Out,
		&DeviceContext
	);

	if (Out == 0)
	{
		// ERROR
		printf("ERROR\n");
		ETERNAL_BREAK();
	}

	if (hr != S_OK)
	{
		// ERROR
		DWORD err = GetLastError();
		printf("ERROR %d:%x\n", err, err);
		ETERNAL_BREAK();
	}

	//hr = _Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&_Debug);
	//ETERNAL_ASSERT(hr == S_OK);
	//hr = _Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//ETERNAL_ASSERT(hr == S_OK);

	//hr = _Debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&_InfoQueue);
	//ETERNAL_ASSERT(hr == S_OK);

	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_APPLICATION_DEFINED, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_MISCELLANEOUS, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_INITIALIZATION, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_CLEANUP, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_COMPILATION, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_STATE_CREATION, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_STATE_SETTING, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_STATE_GETTING, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_RESOURCE_MANIPULATION, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_EXECUTION, TRUE);
	//_InfoQueue->SetBreakOnCategory(D3D11_MESSAGE_CATEGORY_SHADER, TRUE);

	//_InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
	//_InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
	//_InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
	//_InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_INFO, TRUE);
	//_InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_MESSAGE, TRUE);

	_MainContext = new D3D11Context(DeviceContext);

	return hr;
}

int D3D11Renderer::_CreateSwapChain()
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

	if ((UINT)(GetAntiAliasing() & 0xffff) < Quality)
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
		ETERNAL_BREAK();
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
	ETERNAL_ASSERT(_Device);
	return _Device;
}

Context* D3D11Renderer::GetMainContext()
{
	ETERNAL_ASSERT(_MainContext);
	return _MainContext;
}

void D3D11Renderer::Flush()
{
	_SwapChain->Present(0, 0);
}

Context* D3D11Renderer::CreateDeferredContext()
{
	ID3D11DeviceContext* DeviceContext;
	_Device->CreateDeferredContext(0, &DeviceContext);
	return new D3D11Context(DeviceContext, true);
}
