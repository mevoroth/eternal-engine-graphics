#include "d3d10/D3D10Renderer.hpp"

#include <cassert>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d10/D3D10Device.hpp"
#include "d3d10/D3D10RenderTarget.hpp"
#include "d3d10/D3D10Camera.hpp"

using namespace Eternal::Graphics;

D3D10Renderer::D3D10Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
	, _device(0)
	, _swapChain(0)
	, _camera(0)
	, _renderTargets(0)
	, _renderTargetsCount(0)
{
	assert(mode != SOFTWARE); // NOT IMPLEMENTED YET

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
	ID3D10Texture2D* backBufferTex = 0;
	_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&backBufferTex);
	_SetBackBuffer(new D3D10RenderTarget(backBufferTex));
}

HRESULT D3D10Renderer::_CreateDevice()
{
	HRESULT hr = D3D10CreateDevice(
		0,
		GetRenderMode() == SOFTWARE ? D3D10_DRIVER_TYPE_SOFTWARE : D3D10_DRIVER_TYPE_HARDWARE,
		0, // SOFTWARE NOT IMPLEMENTED
		0
#ifdef ETERNAL_DEBUG
		| D3D10_CREATE_DEVICE_DEBUG
#endif
#ifdef ETERNAL_D3D_DEBUG
		| D3D10_CREATE_DEVICE_SINGLETHREADED
#endif
		,
		D3D10_SDK_VERSION,
		&_device
	);

	if (hr != S_OK)
	{
		// ERROR
		DWORD err = GetLastError();
		printf("ERROR %d:%x\n", err, err);
	}

	return hr;
}

HRESULT D3D10Renderer::_CreateSwapChain()
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
	swapChainDesc.OutputWindow = static_cast<D3D10Device*>(Device::Get())->GetWindow();
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
		//char str[256];
		//DWORD err = GetLastError();
		//sprintf_s(str, "ERROR: %d:%x\n", err, err);
		//OutputDebugString(str);
	}

	return hr;
}

ID3D10Device* D3D10Renderer::GetDevice()
{
	assert(_device);
	return _device;
}

void D3D10Renderer::Flush()
{
	_swapChain->Present(0, 0);
}

void D3D10Renderer::AttachCamera(_In_ Camera* camera)
{
	_camera = camera;
}

void D3D10Renderer::AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count)
{
	assert(renderTargets);
	assert(count > 0);
	_renderTargets = renderTargets;
	_renderTargetsCount = count;
}

void D3D10Renderer::AttachMaterial(_In_ Material* material)
{
	_material = material;
}

void D3D10Renderer::ClearRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count)
{
	assert(renderTargets);
	assert(count > 0);
	for (int i = 0; i < count; ++i)
	{
		renderTargets[i]->Clear();
	}
}
void D3D10Renderer::SetBlendMode(_In_ BlendState* blendMode)
{
	_blendMode = blendMode;
	_blendMode->Apply();
}

void D3D10Renderer::SetVBO(_In_ VertexBuffer* vbo)
{
	_vertexBuffer = vbo;
}

void D3D10Renderer::DrawIndexed(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
	_In_ const uint16_t indices[], _In_ int indicesCount)
{
	_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = vertexSize;
	UINT offset = 0;

	ID3D10Buffer* vertexBuffer = 0;

	D3D10_BUFFER_DESC verticesBufferDesc;
	verticesBufferDesc.ByteWidth = vertexSize * verticesCount;
	verticesBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	verticesBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	verticesBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	verticesBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA verticesData;
	verticesData.pSysMem = vertices;
	verticesData.SysMemPitch = 0;
	verticesData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&verticesBufferDesc, &verticesData, &vertexBuffer);
	assert(hr == S_OK);

	ID3D10Buffer* indicesBuffer = 0;

	D3D10_BUFFER_DESC indicesBufferDesc;
	indicesBufferDesc.ByteWidth = sizeof(uint16_t)* indicesCount;
	indicesBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	indicesBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indicesBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	indicesBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA indicesData;
	indicesData.pSysMem = indices;
	indicesData.SysMemPitch = 0;
	indicesData.SysMemSlicePitch = 0;

	hr = _device->CreateBuffer(&indicesBufferDesc, &indicesData, &indicesBuffer);
	assert(hr == S_OK);

	ID3D10Buffer* matrixBuffer = 0;

	D3D10_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA matrixData;
	MatrixBuffer matrixStruct;
	matrixStruct.model = XMMatrixIdentity();//XMLoadFloat4x4();
	matrixStruct.projection = _camera->GetProjectionMatrix();
	matrixStruct.view = _camera->GetViewMatrix();

	matrixData.pSysMem = &matrixStruct;
	matrixData.SysMemPitch = 0;
	matrixData.SysMemSlicePitch = 0;
	hr = _device->CreateBuffer(&matrixBufferDesc, &matrixData, &matrixBuffer);
	assert(hr == S_OK);

	/*FOR TESTING PURPOSE*/

	//if (verticesCount == 4)
	//{
	//	// DEBUG
	//	D3D11VertexPosNormTex* transformeds = (D3D11VertexPosNormTex*)vertices;
	//	XMVECTOR tverts[4];
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		tverts[i] = XMVector4Transform(transformeds[i].Pos, matrixStruct.model);
	//		tverts[i] = XMVector4Transform(tverts[i], matrixStruct.view);
	//		tverts[i] = XMVector4Transform(tverts[i], matrixStruct.projection);
	//		char str[256];
	//		sprintf_s(str, "V%d : { %f, %f, %f }\n", i, tverts[i].m128_f32[0], tverts[i].m128_f32[1], tverts[i].m128_f32[2]);
	//		OutputDebugString(str);
	//	}
	//}

	//D3D11_RASTERIZER_DESC rasterizerDesc;
	//rasterizerDesc.AntialiasedLineEnable = FALSE;
	//rasterizerDesc.CullMode = D3D11_CULL_NONE; // D3D11_CULL_FRONT or D3D11_CULL_NONE D3D11_CULL_BACK
	//rasterizerDesc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_SOLID  D3D11_FILL_WIREFRAME
	//rasterizerDesc.DepthBias = 0;
	//rasterizerDesc.DepthBiasClamp = 0.0f;
	//rasterizerDesc.DepthClipEnable = TRUE;
	//rasterizerDesc.FrontCounterClockwise = TRUE;
	//rasterizerDesc.MultisampleEnable = TRUE;
	//rasterizerDesc.ScissorEnable = FALSE;
	//rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//ID3D11RasterizerState* rasterizerState;
	//_device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	//_deviceContext->RSSetState(rasterizerState);

	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//depthStencilDesc.DepthEnable = FALSE;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//depthStencilDesc.StencilEnable = FALSE;
	//depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	//depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	//ID3D11DepthStencilState* depthStencilState;
	//_device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	//_deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	/*END FOR TESTING PURPOSE*/

	ID3D10RenderTargetView** renderTargetsBuffer = new ID3D10RenderTargetView*[_renderTargetsCount];
	for (int i = 0; i < _renderTargetsCount; ++i)
	{
		renderTargetsBuffer[i] = ((D3D10RenderTarget*)_renderTargets[i])->GetD3D10RenderTarget();
	}
	_device->OMSetRenderTargets(_renderTargetsCount, renderTargetsBuffer, 0);

	_device->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	_device->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R16_UINT, 0);

	_device->VSSetConstantBuffers(0, 1, &matrixBuffer);

	_device->DrawIndexed(indicesCount, 0, 0);
	//_deviceContext->Draw(verticesCount, 0);

	vertexBuffer->Release();
	vertexBuffer = 0;

	indicesBuffer->Release();
	indicesBuffer = 0;

	matrixBuffer->Release();
	matrixBuffer = 0;

	delete[] renderTargetsBuffer;
}
