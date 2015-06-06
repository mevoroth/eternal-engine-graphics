#include "d3d11/D3D11PixelShader.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11PixelShader::D3D11PixelShader(_In_ const string& Name, _In_ const string& Src, _In_ ID3D11ClassLinkage* ClassLinkage)
	: D3D11Shader(Name, Src, "PS", "ps_5_0")
{
	assert(_Program);

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreatePixelShader(
		_Program->GetBufferPointer(),
		_Program->GetBufferSize(),
		ClassLinkage,
		&_Shader
	);
	assert(hr == S_OK);
}

void* D3D11PixelShader::GetD3D11Shader()
{
	assert(_Shader);
	return _Shader;
}
