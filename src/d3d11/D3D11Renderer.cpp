#include "d3d11/D3D11Renderer.hpp"

#include <cassert>
#include <dxgi.h>
#include <cstdio>
#include <WinBase.h>

#include "d3d11/D3D11Device.hpp"
#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11Camera.hpp"
#include "d3d11/D3D11Material.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"

using namespace Eternal::Graphics;

D3D11Renderer::D3D11Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: Renderer(mode, aa)
	, _Device(0)
	, _DeviceContext(0)
	, _SwapChain(0)
	, _Camera(0)
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
	ID3D11Texture2D* BackBufferTex = 0;
	_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBufferTex);
	_SetBackBuffer(new D3D11RenderTarget(BackBufferTex));
	_DeferredMaterial = new D3D11Material();
	D3D11InputLayout InputLayout((D3D11InputLayout::VertexDataType)(D3D11InputLayout::POSITION_T /*| D3D11InputLayout::NORMAL_T*/ | D3D11InputLayout::TEXCOORD_T));
	D3D11VertexShader VS("default", "default.vs.hlsl", InputLayout);
	D3D11PixelShader PS("default", "default.ps.hlsl");
	dynamic_cast<D3D11Material*>(_DeferredMaterial)->AttachInputLayout(&InputLayout);
	_DeferredMaterial->AttachVertexShader(&VS);
	_DeferredMaterial->AttachPixelShader(&PS);
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
		&_DeviceContext
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
		// ERROR
		//char str[256];
		//DWORD err = GetLastError();
		//sprintf_s(str, "ERROR: %d:%x\n", err, err);
		//OutputDebugString(str);
	}

	return hr;
}

void D3D11Renderer::_Settings()
{
	// Always draw triangles
	_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11Device* D3D11Renderer::GetDevice()
{
	assert(_Device);
	return _Device;
}

ID3D11DeviceContext* D3D11Renderer::GetDeviceContext()
{
	assert(_DeviceContext);
	return _DeviceContext;
}

void D3D11Renderer::Flush()
{
	_SwapChain->Present(0, 0);
}

void D3D11Renderer::AttachCamera(_In_ Camera* camera)
{
	_Camera = camera;
}

void D3D11Renderer::AttachRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int Count)
{
	assert(RenderTargets);
	assert(Count > 0);
	_RenderTargets = RenderTargets;
	_RenderTargetsCount = Count;
}

void D3D11Renderer::AttachMaterial(_In_ Material* Material)
{
	if (_Material != Material)
	{
		dynamic_cast<D3D11Material*>(_Material)->Unbind();
		_Material = Material;
	}
}

void D3D11Renderer::ClearRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int Count)
{
	assert(RenderTargets);
	assert(Count > 0);
	for (int i = 0; i < Count; ++i)
	{
		RenderTargets[i]->Clear();
	}
}
void D3D11Renderer::SetBlendMode(_In_ BlendState* blendMode)
{
	_BlendMode = blendMode;
	_BlendMode->Apply();
}

void D3D11Renderer::SetVBO(_In_ VertexBuffer* vbo)
{
	_vertexBuffer = vbo;
}

void D3D11Renderer::DrawIndexed(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
	_In_ const uint16_t indices[], _In_ int indicesCount)
{
	_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = vertexSize;
	UINT offset = 0;

	ID3D11Buffer* vertexBuffer = 0;

	D3D11_BUFFER_DESC verticesBufferDesc;
	verticesBufferDesc.ByteWidth = vertexSize * verticesCount;
	verticesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	verticesBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	verticesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	verticesBufferDesc.MiscFlags = 0;
	verticesBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA verticesData;
	verticesData.pSysMem = vertices;
	verticesData.SysMemPitch = 0;
	verticesData.SysMemSlicePitch = 0;

	HRESULT hr = _Device->CreateBuffer(&verticesBufferDesc, &verticesData, &vertexBuffer);
	assert(hr == S_OK);

	ID3D11Buffer* indicesBuffer = 0;

	D3D11_BUFFER_DESC indicesBufferDesc;
	indicesBufferDesc.ByteWidth = sizeof(uint16_t) * indicesCount;
	indicesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indicesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indicesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indicesBufferDesc.MiscFlags = 0;
	indicesBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indicesData;
	indicesData.pSysMem = indices;
	indicesData.SysMemPitch = 0;
	indicesData.SysMemSlicePitch = 0;

	hr = _Device->CreateBuffer(&indicesBufferDesc, &indicesData, &indicesBuffer);
	assert(hr == S_OK);

	ID3D11Buffer* matrixBuffer = 0;

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA matrixData;
	MatrixBuffer matrixStruct;
	matrixStruct.model = XMMatrixIdentity();//XMLoadFloat4x4();
	matrixStruct.projection = _Camera->GetProjectionMatrix();
	matrixStruct.view = _Camera->GetViewMatrix();

	matrixData.pSysMem = &matrixStruct;
	matrixData.SysMemPitch = 0;
	matrixData.SysMemSlicePitch = 0;
	hr = _Device->CreateBuffer(&matrixBufferDesc, &matrixData, &matrixBuffer);
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

	ID3D11RenderTargetView** renderTargetsBuffer = new ID3D11RenderTargetView*[_RenderTargetsCount];
	for (int i = 0; i < _RenderTargetsCount; ++i)
	{
		renderTargetsBuffer[i] = ((D3D11RenderTarget*)_RenderTargets[i])->GetD3D11RenderTarget();
	}
	_DeviceContext->OMSetRenderTargets(_RenderTargetsCount, renderTargetsBuffer, 0);

	_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	_DeviceContext->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R16_UINT, 0);

	_DeviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	_DeviceContext->DrawIndexed(indicesCount, 0, 0);
	//_deviceContext->Draw(verticesCount, 0);

	vertexBuffer->Release();
	vertexBuffer = 0;

	indicesBuffer->Release();
	indicesBuffer = 0;

	matrixBuffer->Release();
	matrixBuffer = 0;

	delete[] renderTargetsBuffer;
}

void D3D11Renderer::DrawDeferred(_In_ const Vertex Vertices[], _In_ int VerticesCount, _In_ size_t VertexSize,
	_In_ const uint16_t Indices[], _In_ int IndicesCount)
{
	UINT Stride = VertexSize;
	UINT Offset = 0;

	ID3D11Buffer* VertexBuffer = 0;

	D3D11_BUFFER_DESC VerticesBufferDesc;
	VerticesBufferDesc.ByteWidth = VertexSize * VerticesCount;
	VerticesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VerticesBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VerticesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VerticesBufferDesc.MiscFlags = 0;
	VerticesBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VerticesData;
	VerticesData.pSysMem = Vertices;
	VerticesData.SysMemPitch = 0;
	VerticesData.SysMemSlicePitch = 0;

	HRESULT hr = _Device->CreateBuffer(&VerticesBufferDesc, &VerticesData, &VertexBuffer);
	assert(hr == S_OK);

	ID3D11Buffer* IndicesBuffer = 0;

	D3D11_BUFFER_DESC IndicesBufferDesc;
	IndicesBufferDesc.ByteWidth = sizeof(uint16_t)* IndicesCount;
	IndicesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	IndicesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndicesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	IndicesBufferDesc.MiscFlags = 0;
	IndicesBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IndicesData;
	IndicesData.pSysMem = Indices;
	IndicesData.SysMemPitch = 0;
	IndicesData.SysMemSlicePitch = 0;

	hr = _Device->CreateBuffer(&IndicesBufferDesc, &IndicesData, &IndicesBuffer);
	assert(hr == S_OK);

	ID3D11Buffer* MatrixBuffer = 0;

	D3D11_BUFFER_DESC MatrixBufferDesc;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA MatrixData;
	D3D11Renderer::MatrixBuffer MatrixStruct;
	MatrixStruct.model = _GetMatrix();
	MatrixStruct.projection = _Camera->GetProjectionMatrix();
	MatrixStruct.view = _Camera->GetViewMatrix();

	MatrixData.pSysMem = &MatrixStruct;
	MatrixData.SysMemPitch = 0;
	MatrixData.SysMemSlicePitch = 0;
	hr = _Device->CreateBuffer(&MatrixBufferDesc, &MatrixData, &MatrixBuffer);
	assert(hr == S_OK);

	_DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(IndicesBuffer, DXGI_FORMAT_R16_UINT, 0);

	_DeviceContext->VSSetConstantBuffers(0, 1, &MatrixBuffer);

	_Material->Apply();

	_DeviceContext->DrawIndexed(IndicesCount, 0, 0);
	//_deviceContext->Draw(verticesCount, 0);

	VertexBuffer->Release();
	VertexBuffer = 0;

	IndicesBuffer->Release();
	IndicesBuffer = 0;

	MatrixBuffer->Release();
	MatrixBuffer = 0;
}

void D3D11Renderer::BeginDeferred()
{
	// Rendertarget0 : col/opacity
	// Rendertarget1 : metallic/specular/roughness
	// Rendertarget2 : emissive
	// Rendertarget3 : normal
	// Rendertarget4 : worldposition
	// Rendertarget5 : AO

	assert(_RenderTargets);
	assert(_RenderTargetsCount >= 5);

	RenderTarget* BackBuffer = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetBackBuffer();
	ClearRenderTargets(&BackBuffer, 1);
	ClearRenderTargets(_RenderTargets, _RenderTargetsCount);

	ID3D11RenderTargetView** RenderTargetsBuffer = new ID3D11RenderTargetView*[_RenderTargetsCount];
	for (int i = 0; i < _RenderTargetsCount; ++i)
	{
		RenderTargetsBuffer[i] = ((D3D11RenderTarget*)_RenderTargets[i])->GetD3D11RenderTarget();
	}
	_DeviceContext->OMSetRenderTargets(_RenderTargetsCount, RenderTargetsBuffer, 0);
}

void D3D11Renderer::EndDeferred()
{
	Vertex Screen[4];
	Screen[0].Pos = NewVector4(0.f, 0.f, 1.f, 1.f);
	Screen[0].Tex = NewVector2(0.f, 0.f);
	Screen[1].Pos = NewVector4(1.f, 0.f, 1.f, 1.f);
	Screen[1].Tex = NewVector2(1.f, 0.f);
	Screen[2].Pos = NewVector4(1.f, 1.f, 1.f, 1.f);
	Screen[2].Tex = NewVector2(1.f, 1.f);
	Screen[3].Pos = NewVector4(0.f, 1.f, 1.f, 1.f);
	Screen[3].Tex = NewVector2(0.f, 1.f);
	
	uint16_t Indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	D3D11Renderer* MainRenderer = dynamic_cast<D3D11Renderer*>(Renderer::Get());
	RenderTarget* BackBuffer = MainRenderer->GetBackBuffer();

	AttachMaterial(_DeferredMaterial);
	_Material->SetTexture("BaseColorBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[0]));
	_Material->SetTexture("MetallicSpecularRoughnessBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[1]));
	_Material->SetTexture("EmissiveBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[2]));
	_Material->SetTexture("NormalBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[3]));
	_Material->SetTexture("WorldPositionBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[4]));
	_Material->SetTexture("AmbientOcclusionBuffer", dynamic_cast<D3D11RenderTarget*>(_RenderTargets[5]));

	MainRenderer->AttachRenderTargets(&BackBuffer, 1);

	DrawIndexed(Screen, 4, sizeof(Vertex), Indices, 6);

	Flush();
}
