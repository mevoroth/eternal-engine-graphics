#include "d3d11/D3D11PixelShader.hpp"

#include "d3d11/D3D11Renderer.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

D3D11PixelShader::D3D11PixelShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines, _In_ ID3D11ClassLinkage* ClassLinkage)
	: D3D11Shader(Name, Src, "PS", "ps_5_0", Defines)
{
	ETERNAL_ASSERT(_Program);

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreatePixelShader(
		_Program->GetBufferPointer(),
		_Program->GetBufferSize(),
		ClassLinkage,
		&_Shader
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11PixelShader::~D3D11PixelShader()
{
	ETERNAL_ASSERT(_Shader);

	_Shader->Release();
	_Shader = nullptr;
}

void* D3D11PixelShader::GetD3D11Shader()
{
	ETERNAL_ASSERT(_Shader);
	return _Shader;
}
