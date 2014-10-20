#include "d3d10/D3D10GeometryShader.hpp"

#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;

D3D10GeometryShader::D3D10GeometryShader(_In_ const string& name, _In_ const string& src)
	: D3D10Shader(name, src, "GS", "gs_4_0")
{
	assert(_program);
}

void D3D10GeometryShader::InstantiateShader(_Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateGeometryShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		(ID3D10GeometryShader**)code
	);
	assert(hr == S_OK);
}
