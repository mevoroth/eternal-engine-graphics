#include "d3d11/D3D11Material.hpp"

#include <cassert>
#include <numeric>

#include "d3d11/D3D11ShaderFactory.hpp"
#include "d3d11/D3D11Renderer.hpp"

using namespace std;
using namespace Eternal::Graphics;

static UINT accSize(const D3D11_INPUT_ELEMENT_DESC& a, const D3D11_INPUT_ELEMENT_DESC& b)
{
	return 0;
}

D3D11Material::D3D11Material()
{
	HRESULT hr = D3D11Renderer::Get()->GetDevice()->CreateClassLinkage(&_dynamicParams);
	assert(hr);
}

void D3D11Material::SetMaterialDesc(_In_ const string& paramName, _In_ const ParamType& format)
{
	D3D11_INPUT_ELEMENT_DESC desc;
	memset(&desc, 0x0, sizeof(D3D11_INPUT_ELEMENT_DESC));

	desc.SemanticName = paramName.c_str();
	desc.SemanticIndex = 0;
	desc.Format = _GetD3DParam(format);
	desc.InputSlot = _matInput.size();
	desc.AlignedByteOffset = (UINT)accumulate(_matInput.cbegin(), _matInput.cend(), accSize);
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.InstanceDataStepRate = 0;

	_matInput.push_back(desc);
}

void D3D11Material::SetVertexShader(_In_ const string& shader)
{
	HRESULT hr;
	ID3DBlob* program = 0;
	D3D11ShaderFactory::Get()->loadVertex(shader, &program);
	hr = D3D11Renderer::Get()->GetDevice()->CreateVertexShader(program, sizeof(_vertex), _dynamicParams, &_vertex);
	assert(hr);
}
void D3D11Material::SetGeometryShader(_In_ const string& shader)
{
	HRESULT hr;
	ID3DBlob* program = 0;
	D3D11ShaderFactory::Get()->loadGeometry(shader, &program);
	hr = D3D11Renderer::Get()->GetDevice()->CreateGeometryShader(program, sizeof(_geometry), _dynamicParams, &_geometry);
	assert(hr);
}
void D3D11Material::SetPixelShader(_In_ const string& shader)
{
	HRESULT hr;
	ID3DBlob* program = 0;
	D3D11ShaderFactory::Get()->loadPixel(shader, &program);
	hr = D3D11Renderer::Get()->GetDevice()->CreatePixelShader(program, sizeof(_pixel), _dynamicParams, &_pixel);
	assert(hr);
}

inline DXGI_FORMAT D3D11Material::_GetD3DParam(const ParamType& paramType) const
{
	switch (paramType)
	{
	case ParamType::FLOAT_T:
		return DXGI_FORMAT_R32_FLOAT;
	case ParamType::TEX_T:
		return DXGI_FORMAT_BC3_UNORM;
	case ParamType::VEC2_T:
		return DXGI_FORMAT_R32G32_FLOAT;
	case ParamType::VEC3_T:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case ParamType::VEC4_T:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ParamType::COLOR_T:
		return DXGI_FORMAT_R8G8B8A8_SNORM;
	default:
		return (DXGI_FORMAT)-1;
	}
}
