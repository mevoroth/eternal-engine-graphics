#include "d3d11/D3D11VertexShader.hpp"

#include <vector>
#include "d3d11/D3D11Renderer.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D11VertexShader::D3D11VertexShader(_In_ const string& name, _In_ const string& src, _Inout_ D3D11InputLayout& InputLayout)
	: D3D11Shader(name, src, "VS", "vs_5_0")
{
	assert(_program);
	vector<D3D11_INPUT_ELEMENT_DESC> Input = InputLayout._Input;
	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateInputLayout(
		&Input[0],
		Input.size(),
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		&InputLayout._InputLayout
	);
	assert(InputLayout._InputLayout);
}

void D3D11VertexShader::InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code)
{
	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateVertexShader(
		_program->GetBufferPointer(),
		_program->GetBufferSize(),
		classLinkage,
		(ID3D11VertexShader**)code
	);
	assert(hr == S_OK);
}
