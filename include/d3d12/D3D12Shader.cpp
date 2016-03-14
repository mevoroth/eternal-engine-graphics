#include "d3d12/D3D12Shader.hpp"

#include "Macros/Macros.hpp"

#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Eternal::Graphics;
using namespace std;

ID3DInclude* D3D12Shader::_IncludeHandler = new D3D12Include();

D3D12Shader::D3D12Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile)
	: Shader(Name)
	, _Program(nullptr)
{
	ETERNAL_ASSERT(_IncludeHandler);
#ifdef ETERNAL_DEBUG
	_CompileFile(Src, Entry, Profile);
#else
	_LoadFile(Src + ".cso");
#endif
}

D3D12Shader::~D3D12Shader()
{
	_Program->Release();
	_Program = nullptr;
}

void D3D12Shader::_CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile)
{
	ID3DBlob* Errors = nullptr;

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
		_Program = nullptr;
		const char* Error = (LPCSTR)Errors->GetBufferPointer();
		OutputDebugString(Error);
		ETERNAL_ASSERT(false);
	}
}

void D3D12Shader::_LoadFile(_In_ const string& shader)
{
	ETERNAL_ASSERT(false);
}

void D3D12Shader::GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& ShaderObj)
{
	ShaderObj.pShaderBytecode = _Program->GetBufferPointer();
	ShaderObj.BytecodeLength = _Program->GetBufferSize();
}

STDMETHODIMP D3D12Shader::D3D12Include::Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	ifstream IncludedFile(pFileName, ios::in | ios::binary | ios::ate);
	if (!IncludedFile)
	{
		ETERNAL_ASSERT(false);
	}

	*pBytes = IncludedFile.tellg();
	*ppData = new char[*pBytes];
	IncludedFile.seekg(0, ios::beg);
	IncludedFile.read((char*)*ppData, *pBytes);
	IncludedFile.close();

	return S_OK;
}

STDMETHODIMP D3D12Shader::D3D12Include::Close(THIS_ LPCVOID pData)
{
	delete[] pData;

	return S_OK;
}
