#include "d3d11/D3D11GeometryShader.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11GeometryShader::D3D11GeometryShader(_In_ const string& name, _In_ const string& src)
	: D3D11Shader(name, src, "GS", "gs_5_0")
{
	assert(_program);
}

void D3D11GeometryShader::InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code)
{
	HRESULT hr = D3D11Renderer::Get()->GetDevice()->CreateGeometryShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		classLinkage,
		(ID3D11GeometryShader**)code
	);
	assert(hr == S_OK);
}
