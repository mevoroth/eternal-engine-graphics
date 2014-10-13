#include "d3d11/D3D11VertexShader.hpp"

#include <vector>
#include "d3d11/D3D11Renderer.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D11VertexShader::D3D11VertexShader(_In_ const string& name, _In_ const string& src, _Inout_ D3D11InputLayout& inputLayout)
	: D3D11Shader(name, src, "VS", "vs_5_0")
{
	assert(_program);
	vector<D3D11_INPUT_ELEMENT_DESC> input = inputLayout._input;
	dynamic_cast<D3D11Renderer*>(D3D11Renderer::Get())->GetDevice()->CreateInputLayout(
		&input[0],
		input.size(),
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		&inputLayout._inputLayout
	);
	assert(inputLayout._inputLayout);
}

void D3D11VertexShader::InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D11Renderer*>(D3D11Renderer::Get())->GetDevice()->CreateVertexShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		classLinkage,
		(ID3D11VertexShader**)code
	);
	assert(hr == S_OK);
}
