#include "d3d11/D3D11ShaderFactory.hpp"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Eternal::Graphics;

D3D11ShaderFactory* D3D11ShaderFactory::_inst = 0;

D3D11ShaderFactory::D3D11ShaderFactory() {}
D3D11ShaderFactory* D3D11ShaderFactory::Get()
{
	if (!_inst)
	{
		_inst = new D3D11ShaderFactory();
	}
	return _inst;
}

void D3D11ShaderFactory::loadVertex(_In_ const string& shader, _Out_ ID3DBlob** blob)
{
	_loadShaderFile(shader, "vs_5_0", blob);
}
void D3D11ShaderFactory::loadGeometry(_In_ const string& shader, _Out_ ID3DBlob** blob)
{
	_loadShaderFile(shader, "gs_5_0", blob);
}
void D3D11ShaderFactory::loadPixel(_In_ const string& shader, _Out_ ID3DBlob** blob)
{
	_loadShaderFile(shader, "ps_5_0", blob);
}

void D3D11ShaderFactory::_compileFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob)
{
	ID3DBlob* errors;

	HRESULT hr = D3DCompileFromFile(
		wstring(shader.cbegin(), shader.cend()).c_str(),
		0,
		0,
		"main",
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
		0,
		blob,
		&errors
	);

	if (hr != S_OK)
	{
		blob = 0;
	}
}

inline void D3D11ShaderFactory::_loadShaderFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob)
{
#ifdef ETERNAL_DEBUG
	_compileFile(shader, profile, blob);
#else
	_loadFile(shader + ".cso", blob);
#endif
}

void D3D11ShaderFactory::_loadFile(_In_ const string& shader, _Out_ ID3DBlob** blob)
{
	HRESULT hr = D3DReadFileToBlob(wstring(shader.cbegin(), shader.cend()).c_str(), blob);
	if (hr != S_OK)
	{
		blob = 0;
	}
}
