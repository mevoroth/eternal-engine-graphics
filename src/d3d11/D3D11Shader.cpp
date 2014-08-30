#include "d3d11/D3D11Shader.hpp"

#include <cassert>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Eternal::Graphics;

D3D11Shader::D3D11Shader(_In_ const string& name, _In_ const string& src, _In_ const string& entry, _In_ const string& profile)
	: Shader(name)
	, _program(0)
{
#ifdef ETERNAL_DEBUG
	_CompileFile(src, entry, profile);
#else
	_LoadFile(shader + ".cso");
#endif
}

void D3D11Shader::_CompileFile(_In_ const string& src, _In_ const string& entry, _In_ const string& profile)
{
	ID3DBlob* errors;

	HRESULT hr = D3DCompileFromFile(
		wstring(src.cbegin(), src.cend()).c_str(),
		0,
		0,
		entry.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
		0,
		&_program,
		&errors
	);

	if (hr != S_OK)
	{
		_program = 0;
	}
}

void D3D11Shader::InstantiateShader(ID3D11ClassLinkage* dynamicParams)
{
	_dynamicParams = dynamicParams;
}

void D3D11Shader::_LoadFile(_In_ const string& shader)
{
	assert(false);
}
