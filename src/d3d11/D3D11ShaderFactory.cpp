#include "d3d11/D3D11ShaderFactory.hpp"

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11GeometryShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"

using namespace Eternal::Graphics;

D3D11ShaderFactory::D3D11ShaderFactory()
	: ShaderFactory()
{
	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateClassLinkage(&_ClassLinkage);
	assert(hr == S_OK);
}

Shader* D3D11ShaderFactory::CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size)
{
	D3D11InputLayout* InputLayoutObj = new D3D11InputLayout(DataType, Size);
	return new D3D11VertexShader(Name, Src, _ClassLinkage, InputLayoutObj);
}

Shader* D3D11ShaderFactory::CreateGeometryShader(_In_ const string& Name, _In_ const string& Src)
{
	return new D3D11GeometryShader(Name, Src, _ClassLinkage);
}

Shader* D3D11ShaderFactory::CreatePixelShader(_In_ const string& Name, _In_ const string& Src)
{
	return new D3D11PixelShader(Name, Src, _ClassLinkage);
}
