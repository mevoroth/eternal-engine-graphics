#include "d3d11/D3D11Material.hpp"

#include <cassert>
#include <numeric>

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Shader.hpp"
#include "d3d11/D3D11VertexShader.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D11Material::D3D11Material()
	: _dynamicParams(0)
	, _vertex(0)
	, _geometry(0)
	, _pixel(0)
{
	D3D11Renderer::Get()->GetDevice()->CreateClassLinkage(&_dynamicParams);
}

void D3D11Material::SetMaterialDesc(_In_ const MaterialProperty& matProperty)
{
	assert(matProperty.Name.size() > 0);
	_matInput.push_back(matProperty);
}

void D3D11Material::AttachInputLayout(_In_ D3D11InputLayout* inputLayout)
{
	_inputLayout = inputLayout;
}

void D3D11Material::Apply()
{
	ID3D11DeviceContext* ctx = D3D11Renderer::Get()->GetDeviceContext();
	ctx->IASetInputLayout(_inputLayout->GetD3D11InputLayout());
	ctx->VSSetShader(_vertex, 0, 0);
	if (_geometry)
	{
		ctx->GSSetShader(_geometry, 0, 0);
	}
	ctx->PSSetShader(_pixel, 0, 0);
}

void D3D11Material::AttachVertexShader(_Inout_ Shader* shader)
{
	if (_vertex)
	{
		_vertex->Release();
		_vertex = 0;
	}
	((D3D11Shader*)shader)->InstantiateShader(_dynamicParams, (void**)&_vertex);
}
void D3D11Material::AttachGeometryShader(_Inout_ Shader* shader)
{
	if (_geometry)
	{
		_geometry->Release();
		_geometry = 0;
	}
	((D3D11Shader*)shader)->InstantiateShader(_dynamicParams, (void**)&_geometry);
}
void D3D11Material::AttachPixelShader(_Inout_ Shader* shader)
{
	if (_pixel)
	{
		_pixel->Release();
		_pixel = 0;
	}
	((D3D11Shader*)shader)->InstantiateShader(_dynamicParams, (void**)&_pixel);
}
