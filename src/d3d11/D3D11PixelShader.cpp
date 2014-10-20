#include "d3d11/D3D11PixelShader.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11PixelShader::D3D11PixelShader(_In_ const string& name, _In_ const string& src)
	: D3D11Shader(name, src, "PS", "ps_5_0")
{
	assert(_program);
}

void D3D11PixelShader::InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreatePixelShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		classLinkage,
		(ID3D11PixelShader**)code
	);
	assert(hr == S_OK);
}
