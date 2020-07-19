#include "d3d10/D3D10VertexShader.hpp"

#include "d3d10/D3D10Renderer.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D10VertexShader::D3D10VertexShader(_In_ const string& name, _In_ const string& src, _Inout_ D3D10InputLayout& inputLayout)
	: D3D10Shader(name, src, "VS", "vs_4_0")
{
	assert(_program);
	vector<D3D10_INPUT_ELEMENT_DESC> input = inputLayout._input;
	const void* ptr = _program->GetBufferPointer();
	int size = _program->GetBufferSize();
	HRESULT hr = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateInputLayout(
		&input[0],
		input.size(),
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		&inputLayout._inputLayout
	);
	assert(inputLayout._inputLayout);
}

void D3D10VertexShader::InstantiateShader(_Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateVertexShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		(ID3D10VertexShader**)code
	);
	assert(hr == S_OK);
}
