#include "d3d11/D3D11Shader.hpp"

#include <cassert>
#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Eternal::Graphics;
using namespace std;

ID3DInclude* D3D11Shader::_IncludeHandler = new D3D11Include();

D3D11Shader::D3D11Shader(_In_ const string& name, _In_ const string& src, _In_ const string& entry, _In_ const string& profile)
	: Shader(name)
	, _program(0)
{
	assert(_IncludeHandler);
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
		_IncludeHandler,
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
		const char* error = (LPCSTR)errors->GetBufferPointer();
		OutputDebugString(error);
		assert(false);
	}
}

void D3D11Shader::_LoadFile(_In_ const string& shader)
{
	assert(false);
}

STDMETHODIMP D3D11Shader::D3D11Include::Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	//if (pParentData)
	//{
	//	assert(false);
	//}

	ifstream IncludedFile(pFileName, ios::in | ios::binary | ios::ate);
	if (!IncludedFile)
	{
		assert(false);
	}

	*pBytes = IncludedFile.tellg();
	*ppData = new char[*pBytes];
	IncludedFile.seekg(0, ios::beg);
	IncludedFile.read((char*)*ppData, *pBytes);
	IncludedFile.close();

	return S_OK;
}

STDMETHODIMP D3D11Shader::D3D11Include::Close(THIS_ LPCVOID pData)
{
	delete[] pData;

	return S_OK;
}
