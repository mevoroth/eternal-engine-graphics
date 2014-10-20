#include "d3d10/D3D10Material.hpp"

#include <cassert>
#include <numeric>

#include "d3d10/D3D10Renderer.hpp"
#include "d3d10/D3D10Shader.hpp"
#include "d3d10/D3D10VertexShader.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D10Material::D3D10Material()
	: _vertex(0)
	, _geometry(0)
	, _pixel(0)
{
}

void D3D10Material::SetMaterialDesc(_In_ const MaterialProperty& matProperty)
{
	assert(matProperty.Name.size() > 0);
	_matInput.push_back(matProperty);
}

void D3D10Material::AttachInputLayout(_In_ D3D10InputLayout* inputLayout)
{
	_inputLayout = inputLayout;
}

void D3D10Material::Apply()
{
	ID3D10Device* ctx = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice();
	ctx->IASetInputLayout(_inputLayout->GetD3D10InputLayout());
	ctx->VSSetShader(_vertex);
	if (_geometry)
	{
		ctx->GSSetShader(_geometry);
	}
	ctx->PSSetShader(_pixel);
}

void D3D10Material::AttachVertexShader(_Inout_ Shader* shader)
{
	if (_vertex)
	{
		_vertex->Release();
		_vertex = 0;
	}
	((D3D10Shader*)shader)->InstantiateShader((void**)&_vertex);
}
void D3D10Material::AttachGeometryShader(_Inout_ Shader* shader)
{
	if (_geometry)
	{
		_geometry->Release();
		_geometry = 0;
	}
	((D3D10Shader*)shader)->InstantiateShader((void**)&_geometry);
}
void D3D10Material::AttachPixelShader(_Inout_ Shader* shader)
{
	if (_pixel)
	{
		_pixel->Release();
		_pixel = 0;
	}
	((D3D10Shader*)shader)->InstantiateShader((void**)&_pixel);
}
