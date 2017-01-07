#include "d3d11/D3D11Shader.hpp"

#include "Macros/Macros.hpp"

//#define WIN32_LEAN_AND_MEAN
//#define VC_EXTRALEAN
//#define WIN32_EXTRA_LEAN
//#include <Windows.h>
#include <cstdlib>
#include <fstream>
//#include <string>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "File/FileFactory.hpp"

using namespace Eternal::Graphics;
using namespace std;

ID3DInclude* D3D11Shader::_IncludeHandler = new D3D11Include();

D3D11Shader::D3D11Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile, _In_ const vector<string>& Defines)
	: Shader(Name)
	, _Program(nullptr)
{
	ETERNAL_ASSERT(_IncludeHandler);
#ifdef ETERNAL_DEBUG
	_CompileFile(Src, Entry, Profile, Defines);
#else
	_LoadFile(Src + ".cso");
#endif
}

D3D11Shader::~D3D11Shader()
{
	_Program->Release();
	_Program = nullptr;
}

void D3D11Shader::_CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile, _In_ const vector<string>& Defines)
{
	ETERNAL_ASSERT(!(Defines.size() % 2)); // Force value for defines

	vector<string>& ShaderPaths = ((D3D11Include*)_IncludeHandler)->_ShaderPaths;
	bool FileExists = false;
	string FilePath;
	for (int ShaderPathIndex = 0; !FileExists && ShaderPathIndex < ShaderPaths.size(); ++ShaderPathIndex)
	{
		FileExists = File::FileExists(ShaderPaths[ShaderPathIndex] + Src);
		if (FileExists)
			FilePath = ShaderPaths[ShaderPathIndex] + Src;
	}
	
	ETERNAL_ASSERT(FileExists);
	ETERNAL_ASSERT(FilePath.size());

	vector<D3D_SHADER_MACRO> Macros;
	for (int DefineIndex = 0, DefineCount = Defines.size() / 2; DefineIndex < DefineCount; ++DefineIndex)
	{
		D3D_SHADER_MACRO Macro;
		Macro.Name = Defines[DefineIndex * 2].c_str();
		Macro.Definition = Defines[DefineIndex * 2 + 1].c_str();
		Macros.push_back(Macro);
	}
	D3D_SHADER_MACRO EmptyMacro;
	EmptyMacro.Name = nullptr;
	EmptyMacro.Definition = nullptr;
	Macros.push_back(EmptyMacro);

	ID3DBlob* Errors;

	HRESULT hr = D3DCompileFromFile(
		wstring(FilePath.cbegin(), FilePath.cend()).c_str(),
		Defines.size() ? &Macros[0] : nullptr,
		_IncludeHandler,
		Entry.c_str(),
		Profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS
#ifdef ETERNAL_DEBUG
		| D3DCOMPILE_DEBUG
		| D3DCOMPILE_SKIP_OPTIMIZATION
		| D3DCOMPILE_ALL_RESOURCES_BOUND
#endif
		,
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

void D3D11Shader::_LoadFile(_In_ const string& shader)
{
	ETERNAL_ASSERT(false);
}

D3D11Shader::D3D11Include::D3D11Include()
	: ID3DInclude()
{
	_ShaderPaths.push_back(".\\");
}

STDMETHODIMP D3D11Shader::D3D11Include::Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	//if (pParentData)
	//{
	//	assert(false);
	//}

	//char FullPathName[256];
	//GetFullPathName(pFileName, strlen(pFileName), FullPathName, nullptr);
	//char Directory[256];
	//_splitpath(FullPathName, nullptr, Directory, nullptr, nullptr);
	//string Include

	bool FileExists = false;
	string FilePath;
	for (int ShaderPathIndex = 0; !FileExists && ShaderPathIndex < _ShaderPaths.size(); ++ShaderPathIndex)
	{
		FileExists = File::FileExists(_ShaderPaths[ShaderPathIndex] + pFileName);
		if (FileExists)
			FilePath = _ShaderPaths[ShaderPathIndex] + pFileName;
	}

	ETERNAL_ASSERT(FileExists);
	ETERNAL_ASSERT(FilePath.size());

	ifstream IncludedFile(FilePath.c_str(), ios::in | ios::binary | ios::ate);

	if (!IncludedFile)
	{
		ETERNAL_ASSERT(false);
	}

	*pBytes = (UINT)IncludedFile.tellg();
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

void D3D11Shader::D3D11Include::RegisterShaderPath(_In_ const string& Path)
{
#ifdef ETERNAL_DEBUG
	for (int ShaderPathIndex = 0; ShaderPathIndex < _ShaderPaths.size(); ++ShaderPathIndex)
	{
		ETERNAL_ASSERT(_ShaderPaths[ShaderPathIndex] != Path);
	}
#endif
	_ShaderPaths.push_back(Path);
}

void D3D11Shader::D3D11Include::CreateDependency(_In_ const string& Src)
{
	//Dependency NewShader;
	//NewShader.FileName = Src;
	//NewShader
	//_ShaderDependencies.push_back(NewShader);
}

vector<string>& D3D11Shader::D3D11Include::GetShaderPaths()
{
	return _ShaderPaths;
}
