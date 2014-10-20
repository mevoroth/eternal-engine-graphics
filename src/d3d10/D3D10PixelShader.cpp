#include "d3d10/D3D10PixelShader.hpp"

#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;

D3D10PixelShader::D3D10PixelShader(_In_ const string& name, _In_ const string& src)
	: D3D10Shader(name, src, "PS", "ps_4_0")
{
	assert(_program);
}

void D3D10PixelShader::InstantiateShader(_Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreatePixelShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		(ID3D10PixelShader**)code
	);
	assert(hr == S_OK);
}
