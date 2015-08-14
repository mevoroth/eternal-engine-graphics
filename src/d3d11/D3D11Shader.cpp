#include "d3d11/D3D11Shader.hpp"

#include <cassert>
#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;
using namespace std;

ID3DInclude* D3D11Shader::_IncludeHandler = new D3D11Include();

D3D11Shader::D3D11Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile)
	: Shader(Name)
	, _Program(0)
{
	ETERNAL_ASSERT(_IncludeHandler);
#ifdef ETERNAL_DEBUG
	_CompileFile(Src, Entry, Profile);
#else
	_LoadFile(Src + ".cso");
#endif
}

D3D11Shader::~D3D11Shader()
{
	_Program->Release();
	_Program = nullptr;
}

void D3D11Shader::_CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile)
{
	ID3DBlob* Errors;

	HRESULT hr = D3DCompileFromFile(
		wstring(Src.cbegin(), Src.cend()).c_str(),
		0,
		_IncludeHandler,
		Entry.c_str(),
		Profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG,
		0,
		&_Program,
		&Errors
	);

	if (hr != S_OK)
	{
		_Program = 0;
		const char* Error = (LPCSTR)Errors->GetBufferPointer();
		OutputDebugString(Error);
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
