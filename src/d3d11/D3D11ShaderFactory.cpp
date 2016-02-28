#include "d3d11/D3D11ShaderFactory.hpp"

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11GeometryShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

D3D11ShaderFactory::D3D11ShaderFactory()
	: ShaderFactory()
{
	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateClassLinkage(&_ClassLinkage);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11ShaderFactory::~D3D11ShaderFactory()
{
	_Clear<D3D11VertexShader>(_VertexShaders);
	_Clear<D3D11GeometryShader>(_GeometryShaders);
	_Clear<D3D11PixelShader>(_PixelShaders);
}

Shader* D3D11ShaderFactory::CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size)
{
	D3D11VertexShader* ShaderObj = _Find<D3D11VertexShader>(_VertexShaders, Name);
	if (ShaderObj)
	{
		return ShaderObj;
	}
	D3D11InputLayout* InputLayoutObj = new D3D11InputLayout(DataType, Size);
	ShaderObj = new D3D11VertexShader(Name, Src, _ClassLinkage, InputLayoutObj);
	_VertexShaders.push_back(ShaderObj);
	return ShaderObj;
}

Shader* D3D11ShaderFactory::CreateVertexShader(_In_ const string& Name, _In_ const string& Src)
{
	D3D11VertexShader* ShaderObj = _Find<D3D11VertexShader>(_VertexShaders, Name);
	if (ShaderObj)
	{
		return ShaderObj;
	}
	D3D11InputLayout* InputLayoutObj = new D3D11InputLayout();
	ShaderObj = new D3D11VertexShader(Name, Src, _ClassLinkage, InputLayoutObj);
	_VertexShaders.push_back(ShaderObj);
	return ShaderObj;
}

Shader* D3D11ShaderFactory::CreateGeometryShader(_In_ const string& Name, _In_ const string& Src)
{
	D3D11GeometryShader* ShaderObj = _Find<D3D11GeometryShader>(_GeometryShaders, Name);
	if (ShaderObj)
	{
		return ShaderObj;
	}
	ShaderObj = new D3D11GeometryShader(Name, Src, _ClassLinkage);
	_GeometryShaders.push_back(ShaderObj);
	return ShaderObj;
}

Shader* D3D11ShaderFactory::CreatePixelShader(_In_ const string& Name, _In_ const string& Src)
{
	D3D11PixelShader* ShaderObj = _Find<D3D11PixelShader>(_PixelShaders, Name);
	if (ShaderObj)
	{
		return ShaderObj;
	}
	ShaderObj = new D3D11PixelShader(Name, Src, _ClassLinkage);
	_PixelShaders.push_back(ShaderObj);
	return ShaderObj;
}
