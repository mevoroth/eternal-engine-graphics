#include "d3d11/D3D11GeometryShader.hpp"

#include "Macros/Macros.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11GeometryShader::D3D11GeometryShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines, _In_ ID3D11ClassLinkage* ClassLinkage)
	: D3D11Shader(Name, Src, "GS", "gs_5_0", Defines)
{
	ETERNAL_ASSERT(_Program);

	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateGeometryShader(
		_Program->GetBufferPointer(),
		_Program->GetBufferSize(),
		ClassLinkage,
		&_Shader
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11GeometryShader::~D3D11GeometryShader()
{
	_Shader->Release();
	_Shader = nullptr;
}

void* D3D11GeometryShader::GetD3D11Shader()
{
	ETERNAL_ASSERT(_Shader);
	return _Shader;
}
