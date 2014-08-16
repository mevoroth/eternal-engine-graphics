#include "D3D11Renderer.hpp"

#include <cassert>

using namespace Eternal::Graphics;

D3D11Renderer::D3D11Renderer(_In_ RenderMode mode)
	: Renderer(mode)
	, _device(0)
{
	assert(mode != SOFTWARE); // NOT IMPLEMENTED YET

	assert(_inst);
	_inst = this;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL out;

	HRESULT hr = D3D11CreateDevice(
		0,
		GetRenderMode() == SOFTWARE ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_SOFTWARE,
		0,
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
		sizeof(featureLevel),
		D3D11_SDK_VERSION,
		&_device,
		&out,
		&_deviceContext
	);

	if (out == 0)
	{
		// ERROR
	}

	if (hr != S_OK)
	{
		// ERROR
	}
}

D3D11Renderer* D3D11Renderer::get()
{
	assert(!_inst);
	return _inst;
}

ID3D11Device* D3D11Renderer::getDevice()
{
	assert(!_device);
	return _device;
}
