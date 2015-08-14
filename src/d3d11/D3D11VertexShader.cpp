#include "d3d11/D3D11VertexShader.hpp"

#include <vector>
#include "d3d11/D3D11Renderer.hpp"
#include "Macros/Macros.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D11VertexShader::D3D11VertexShader(_In_ const string& Name, _In_ const string& Src, _In_ ID3D11ClassLinkage* ClassLinkage, _Inout_ D3D11InputLayout* InputLayout)
	: D3D11Shader(Name, Src, "VS", "vs_5_0")
{
	ETERNAL_ASSERT(_Program);

	ETERNAL_ASSERT(InputLayout);

	_InputLayout = InputLayout;

	vector<D3D11_INPUT_ELEMENT_DESC>& Input = InputLayout->_Input;
	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateInputLayout(
		&Input[0],
		Input.size(),
		_Program->GetBufferPointer(),
		_Program->GetBufferSize(),
		&InputLayout->_InputLayout
	);
	ETERNAL_ASSERT(InputLayout->_InputLayout);

	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateVertexShader(
		_Program->GetBufferPointer(),
		_Program->GetBufferSize(),
		ClassLinkage,
		&_Shader
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11VertexShader::~D3D11VertexShader()
{
	ETERNAL_ASSERT(_Shader);
	ETERNAL_ASSERT(_InputLayout);

	delete _InputLayout;
	_InputLayout = nullptr;

	_Shader->Release();
	_Shader = nullptr;
}

void* D3D11VertexShader::GetD3D11Shader()
{
	ETERNAL_ASSERT(_Shader);
	return _Shader;
}
