#include "d3d12/D3D12Shader.hpp"

#include "GraphicsSettings.hpp"
#include "Graphics/ShaderType.hpp"
#include "File/FilePath.hpp"

#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Eternal::File;
using namespace std;

namespace Eternal
{
	namespace Graphics
	{

		constexpr char* D3D12_SHADER_ENTRIES[] =
		{
			"VS",
			"HS",
			"DS",
			"GS",
			"PS",
			"CS"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_ENTRIES) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader entries");

		constexpr char* D3D12_SHADER_PROFILES[] =
		{
			"vs_5_1",
			"hs_5_1",
			"ds_5_1",
			"gs_5_1",
			"ps_5_1",
			"cs_5_1"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_PROFILES) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader profiles");

		ID3DInclude* D3D12Shader::_IncludeHandler = new D3D12Include();

		D3D12Shader::D3D12Shader(_In_ const string& Name, _In_ const string& FileName, _In_ const ShaderType& Stage, _In_ const vector<string>& Defines /* = vector<string>() */)
			: Shader(Name)
			, _Program(nullptr)
		{
			ETERNAL_ASSERT(_IncludeHandler);
		#if defined(ETERNAL_DEBUG) && defined(ETERNAL_USE_DEBUG_SHADERS)
			_CompileFile(FileName, D3D12_SHADER_ENTRIES[static_cast<int32_t>(Stage)], D3D12_SHADER_PROFILES[static_cast<int32_t>(Stage)], Defines);
		#else
			_LoadFile(FileName + ".cso");
		#endif
		}

		D3D12Shader::D3D12Shader(_In_ GraphicsContext& Context, const ShaderCreateInformation& CreateInformation)
			: D3D12Shader(CreateInformation.Name,
						  CreateInformation.FileName,
						  CreateInformation.Stage,
						  CreateInformation.Defines)
		{
		}

		D3D12Shader::~D3D12Shader()
		{
			_Program->Release();
			_Program = nullptr;
		}

		void D3D12Shader::_CompileFile(_In_ const string& FileName, _In_ const char* Entry, _In_ const char* Profile, _In_ const vector<string>& Defines)
		{
			ETERNAL_ASSERT(!(Defines.size() % 2)); // Force value for defines

			ID3DBlob* Errors = nullptr;

			string FullPathSource = FilePath::Find(FileName, FileType::SHADERS);

			vector<D3D_SHADER_MACRO> Macros;
			Macros.resize((Defines.size() / 2) + 1);
			uint32_t DefineIndex = 0;
			for (; DefineIndex < Macros.size() - 1; ++DefineIndex)
			{
				Macros[DefineIndex].Name		= Defines[DefineIndex * 2].c_str();
				Macros[DefineIndex].Definition	= Defines[DefineIndex * 2 + 1].c_str();
			}
			// Empty macro (eq. end of array)
			Macros[DefineIndex].Name		= nullptr;
			Macros[DefineIndex].Definition	= nullptr;

			HRESULT hr = D3DCompileFromFile(
				wstring(FullPathSource.cbegin(), FullPathSource.cend()).c_str(),
				Macros.data(),
				_IncludeHandler,
				Entry,
				Profile,
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
				ETERNAL_BREAK();
			}
		}

		void D3D12Shader::_LoadFile(_In_ const string& ShaderFile)
		{
			ETERNAL_BREAK();
		}

		void D3D12Shader::GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode)
		{
			OutShaderByteCode.pShaderBytecode = _Program->GetBufferPointer();
			OutShaderByteCode.BytecodeLength = _Program->GetBufferSize();
		}

		STDMETHODIMP D3D12Shader::D3D12Include::Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			string IncludeSrc = pFileName;
			string FullPathSrc = FilePath::Find(IncludeSrc, FileType::SHADERS);
			ifstream IncludedFile(FullPathSrc.c_str(), ios::in | ios::binary | ios::ate);
	
			if (!IncludedFile)
			{
				ETERNAL_BREAK();
			}

			*pBytes = (UINT)IncludedFile.tellg();
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
	}
}
