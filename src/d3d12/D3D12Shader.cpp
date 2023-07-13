#include "d3d12/D3D12Shader.hpp"

#include "GraphicsSettings.hpp"
#include "Graphics/ShaderType.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "File/FilePath.hpp"
#include "File/FileFactory.hpp"
#include "File/File.hpp"
#include "Log/Log.hpp"
#include "d3d12/D3D12Utils.hpp"

#include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <locale>
#include <codecvt>

#include "directxcompiler/dxcapi.h"

using namespace Eternal::FileSystem;
using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		enum class ShaderCompilerType
		{
			SHADER_COMPILER_TYPE_FXC = 0,
			SHADER_COMPILER_TYPE_DXC
		};

		static constexpr ShaderCompilerType ShaderCompiler = ShaderCompilerType::SHADER_COMPILER_TYPE_FXC;

		static constexpr char* D3D12_SHADER_ENTRIES_FXC[] =
		{
			"ShaderVertex",
			"ShaderHull",
			"ShaderDomain",
			"ShaderGeometry",
			"ShaderPixel",
			"ShaderCompute",
			"ShaderMesh",
			"ShaderAmplification"
		};
		static constexpr wchar_t* D3D12_SHADER_ENTRIES_DXC[] =
		{
			L"ShaderVertex",
			L"ShaderHull",
			L"ShaderDomain",
			L"ShaderGeometry",
			L"ShaderPixel",
			L"ShaderCompute",
			L"ShaderMesh",
			L"ShaderAmplification"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_ENTRIES_FXC) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader entries");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_ENTRIES_DXC) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader entries");

		static constexpr char* D3D12_SHADER_PROFILES_FXC[] =
		{
			"vs_5_1",
			"hs_5_1",
			"ds_5_1",
			"gs_5_1",
			"ps_5_1",
			"cs_5_1",
			"",
			""
		};
		static constexpr wchar_t* D3D12_SHADER_PROFILES_DXC[] =
		{
			L"vs_6_5",
			L"hs_6_5",
			L"ds_6_5",
			L"gs_6_5",
			L"ps_6_5",
			L"cs_6_5",
			L"ms_6_5",
			L"as_6_5"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_PROFILES_FXC) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader profiles");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_SHADER_PROFILES_DXC) == static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT), "Mismatch between abstraction and d3d12 shader profiles");

		//////////////////////////////////////////////////////////////////////////

		static constexpr wchar_t DXC_ARG_INCLUDE_PATH[] = L"-I";

		//////////////////////////////////////////////////////////////////////////

		struct D3D12IncludeCommon
		{
			D3D12IncludeCommon(_Inout_ GraphicsContext& InOutContext)
				: Context(InOutContext)
			{
			}

			void SetCurrentShader(_In_ Shader* InShader)
			{
				ETERNAL_ASSERT(!CurrentShader);
				CurrentShader = InShader;
			}

			void ClearCurrentShader()
			{
				CurrentShader = nullptr;
			}

			GraphicsContext& Context;
			Shader* CurrentShader = nullptr;
		};

		struct D3D12IncludeFXC
			: public ID3DInclude
			, public D3D12IncludeCommon
		{
		public:
			D3D12IncludeFXC(_Inout_ GraphicsContext& InOutContext)
				: D3D12IncludeCommon(InOutContext)
			{
			}

			virtual STDMETHODIMP Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
			{
				string IncludeSource = pFileName;
				string IncludeFullPathSource = FilePath::Find(IncludeSource, FileType::FILE_TYPE_SHADERS);
				FilePath::NormalizePath(IncludeFullPathSource);

				Context.GetPipelineDependency().RegisterShaderDependency(CurrentShader, IncludeFullPathSource);

				FileContent Content = LoadFileToMemory(IncludeFullPathSource);
				*pBytes	= static_cast<UINT>(Content.Size);
				*ppData = Content.Content;

				return S_OK;
			}
			virtual STDMETHODIMP Close(THIS_ LPCVOID pData) override
			{
				delete[] pData;

				return S_OK;
			}
		};

		//////////////////////////////////////////////////////////////////////////

		struct D3D12IncludeDXC
			: public IDxcIncludeHandler
			, public D3D12IncludeCommon
		{
		public:
			D3D12IncludeDXC(_Inout_ GraphicsContext& InOutContext, _In_ IDxcUtils* InDxcUtils, _In_ IDxcIncludeHandler* InDefaultIncludeHandler)
				: D3D12IncludeCommon(InOutContext)
				, _DxcUtils(InDxcUtils)
				, _DxcIncludeHandlerDefault(InDefaultIncludeHandler)
			{
			}

			virtual HRESULT STDMETHODCALLTYPE LoadSource(
				_In_z_ LPCWSTR pFilename,                                 // Candidate filename.
				_COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource  // Resultant source object for included file, nullptr if not found.
			) override
			{
				ETERNAL_BREAK();

				IDxcBlobEncoding* Encoding = nullptr;
				wstring IncludeSourceUTF8(pFilename);
				string IncludeSource = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(IncludeSourceUTF8);
				string ShaderFullPathSource = FilePath::Find(IncludeSource, FileType::FILE_TYPE_SHADERS);
				//wstring FullPathSourceUTF8 = wstring(FullPathSource.begin(), FullPathSource.end());

				//HRESULT hr = _DxcUtils->LoadFile(IncludeSourceUTF8.c_str(), nullptr, &Encoding);
				//*ppIncludeSource = hr == S_OK ? Encoding : nullptr;
				//ETERNAL_ASSERT(hr == S_OK);
				
				ifstream IncludedFile(ShaderFullPathSource.c_str(), ios::in | ios::binary | ios::ate);

				if (!IncludedFile)
				{
					//ETERNAL_BREAK();
					*ppIncludeSource = nullptr;
					return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
				}

				UINT Bytes = (UINT)IncludedFile.tellg();
				char* Data = new char[Bytes];
				IncludedFile.seekg(0, ios::beg);
				IncludedFile.read((char*)Data, Bytes);
				IncludedFile.close();
				
				_DxcUtils->CreateBlob(Data, Bytes, 0, &Encoding);
				delete[] Data;

				*ppIncludeSource = Encoding;
				return S_OK;
			}

			virtual HRESULT QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override
			{
				return _DxcIncludeHandlerDefault->QueryInterface(riid, ppvObject);
			}

			virtual ULONG STDMETHODCALLTYPE AddRef( void) override
			{
				return 0;
			}

			virtual ULONG STDMETHODCALLTYPE Release( void) override
			{
				return 0;
			}

		private:
			IDxcUtils*			_DxcUtils					= nullptr;
			IDxcIncludeHandler*	_DxcIncludeHandlerDefault	= nullptr;
		};

		//////////////////////////////////////////////////////////////////////////

		D3D12IncludeFXC*	D3D12Shader::_FxcIncludeHandler			= nullptr;
		D3D12IncludeDXC*	D3D12Shader::_DxcIncludeHandler			= nullptr;
		IDxcIncludeHandler*	D3D12Shader::_DxcIncludeHandlerDefault	= nullptr;
		IDxcUtils*			D3D12Shader::_DxcUtils					= nullptr;
		IDxcCompiler3*		D3D12Shader::_DxcCompiler				= nullptr;

		void D3D12Shader::Initialize(_Inout_ GraphicsContext& InOutContext)
		{
			using namespace Eternal::Graphics::D3D12;

			switch (ShaderCompiler)
			{
			case ShaderCompilerType::SHADER_COMPILER_TYPE_FXC:
			{
				ETERNAL_ASSERT(!_FxcIncludeHandler);
				_FxcIncludeHandler = new D3D12IncludeFXC(InOutContext);
			} break;
			case ShaderCompilerType::SHADER_COMPILER_TYPE_DXC:
			{
				VerifySuccess(
					DxcCreateInstance(
						CLSID_DxcUtils,
						__uuidof(IDxcUtils),
						reinterpret_cast<void**>(&_DxcUtils)
					)
				);

				VerifySuccess(_DxcUtils->CreateDefaultIncludeHandler(&_DxcIncludeHandlerDefault));

				_DxcIncludeHandler = new D3D12IncludeDXC(InOutContext, _DxcUtils, _DxcIncludeHandlerDefault);

				VerifySuccess(
					DxcCreateInstance(
						CLSID_DxcCompiler,
						__uuidof(IDxcCompiler3),
						reinterpret_cast<void**>(&_DxcCompiler)
					)
				);
			} break;
			}
		}

		void D3D12Shader::Destroy()
		{
			switch (ShaderCompiler)
			{
			case ShaderCompilerType::SHADER_COMPILER_TYPE_FXC:
			{
				ETERNAL_ASSERT(_FxcIncludeHandler);
				delete _FxcIncludeHandler;
				_FxcIncludeHandler = nullptr;
			} break;
			case ShaderCompilerType::SHADER_COMPILER_TYPE_DXC:
			{
				delete _DxcIncludeHandler;
				_DxcIncludeHandler = nullptr;

				_DxcIncludeHandlerDefault->Release();
				_DxcIncludeHandlerDefault = nullptr;

				_DxcUtils->Release();
				_DxcUtils = nullptr;
			} break;
			}
		}

		D3D12Shader::D3D12Shader(_In_ GraphicsContext& InOutContext, _In_ const ShaderCreateInformation& InCreateInformation)
			: Shader(InOutContext, InCreateInformation)
			, _FxcProgram(nullptr)
			, _DxcProgram(nullptr)
		{
			switch (ShaderCompiler)
			{
			case ShaderCompilerType::SHADER_COMPILER_TYPE_FXC:
			{
				ETERNAL_ASSERT(_FxcIncludeHandler);
			} break;
			case ShaderCompilerType::SHADER_COMPILER_TYPE_DXC:
			{
				ETERNAL_ASSERT(_DxcIncludeHandler);
				ETERNAL_ASSERT(_DxcIncludeHandlerDefault);
				ETERNAL_ASSERT(_DxcUtils);
				ETERNAL_ASSERT(_DxcCompiler);
			} break;
			}
#if ETERNAL_USE_DEBUG_SHADERS
			_CompileFile(
				InCreateInformation.FileName,
				static_cast<uint32_t>(InCreateInformation.Stage),
				InCreateInformation.Defines
			);
#else
			_LoadFile(InFileName + ".cso");
#endif
		}

		D3D12Shader::~D3D12Shader()
		{
			if (IsShaderCompiled())
			{
				switch (ShaderCompiler)
				{
				case ShaderCompilerType::SHADER_COMPILER_TYPE_FXC:
				{
					_FxcProgram->Release();
					_FxcProgram = nullptr;
				} break;
				case ShaderCompilerType::SHADER_COMPILER_TYPE_DXC:
				{
					_DxcProgram->Release();
					_DxcProgram = nullptr;
				} break;
				}
			}
		}

		void D3D12Shader::_CompileFile(_In_ const string& InFileName, _In_ uint32_t InShaderStageInt, _In_ const vector<string>& InDefines)
		{
			using namespace Eternal::Graphics::D3D12;

			ETERNAL_ASSERT(!(InDefines.size() % 2)); // Force value for defines

			string FullPathSource = FilePath::Find(InFileName, FileType::FILE_TYPE_SHADERS);
			FileContent ShaderSourceCode = LoadFileToMemory(FullPathSource);

			string ShaderFileContent = R"HLSLINCLUDE(
				#include "ShadersReflection/HLSLReflection.hpp"
			)HLSLINCLUDE";
			ShaderFileContent += reinterpret_cast<const char*>(ShaderSourceCode.Content);

			switch (ShaderCompiler)
			{
			case ShaderCompilerType::SHADER_COMPILER_TYPE_FXC:
			{
				ID3DBlob* Errors = nullptr;

				uint32_t MacrosCount = static_cast<uint32_t>(InDefines.size()) / 2;
				vector<D3D_SHADER_MACRO> Macros;
				Macros.reserve(MacrosCount + 256);
				for (uint32_t DefineIndex = 0; DefineIndex < MacrosCount; ++DefineIndex)
				{
					Macros.push_back({});
					D3D_SHADER_MACRO& CurrentMacro = Macros.back();

					CurrentMacro.Name		= InDefines[DefineIndex * 2].c_str();
					CurrentMacro.Definition	= InDefines[DefineIndex * 2 + 1].c_str();
				}

				D3D_SHADER_MACRO PlatformMacro;
				PlatformMacro.Name			= "PLATFORM_DX12";
				PlatformMacro.Definition	= "1";
				Macros.push_back(PlatformMacro);

				// Empty macro (eq. end of array)
				D3D_SHADER_MACRO EndOfArrayMacro;
				EndOfArrayMacro.Name		= nullptr;
				EndOfArrayMacro.Definition	= nullptr;
				Macros.push_back(EndOfArrayMacro);

				_FxcIncludeHandler->SetCurrentShader(this);

				HRESULT hr = D3DCompile(
					ShaderFileContent.c_str(),
					ShaderFileContent.size(),
					InFileName.c_str(),
					Macros.data(),
					_FxcIncludeHandler,
					D3D12_SHADER_ENTRIES_FXC[InShaderStageInt],
					D3D12_SHADER_PROFILES_FXC[InShaderStageInt],
					D3DCOMPILE_ENABLE_STRICTNESS | /*D3DCOMPILE_WARNINGS_ARE_ERRORS |*/ D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION,
					0,
					&_FxcProgram,
					&Errors
				);

				_FxcIncludeHandler->ClearCurrentShader();

				if (hr != S_OK)
				{
					_FxcProgram = nullptr;
					const char* Error = (LPCSTR)Errors->GetBufferPointer();
					LogWrite(LogError, LogShaders, GetFileName());
					LogWrite(LogError, LogShaders, Error);
				}

				if (Errors)
				{
					Errors->Release();
					Errors = nullptr;
				}
			} break;
			case ShaderCompilerType::SHADER_COMPILER_TYPE_DXC:
			{
				ETERNAL_ASSERT(_DxcUtils);
				ETERNAL_ASSERT(_DxcCompiler);

				vector<LPCWSTR> Arguments;
				vector<DxcDefine> Macros;
				vector<wstring> InDefinesUTF8;

				{
					Arguments.reserve(256);
					Arguments.push_back(DXC_ARG_DEBUG);
					Arguments.push_back(DXC_ARG_WARNINGS_ARE_ERRORS);
					Arguments.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
					Arguments.push_back(DXC_ARG_ENABLE_STRICTNESS);

					//const vector<wstring>& ShadersIncludePaths = FilePath::GetFolderPathsUTF8(FileType::FILE_TYPE_SHADERS);
					//for (uint32_t PathIndex = 0; PathIndex < ShadersIncludePaths.size(); ++PathIndex)
					//{
					//	Arguments.push_back(DXC_ARG_INCLUDE_PATH);
					//	Arguments.push_back(ShadersIncludePaths[PathIndex].c_str());
					//}
					Arguments.push_back(DXC_ARG_INCLUDE_PATH);
					Arguments.push_back(L".\\");
				}
				{
					uint32_t MacrosCount = static_cast<uint32_t>(InDefines.size()) / 2;
					Macros.reserve(MacrosCount + 256);
					InDefinesUTF8.resize(InDefines.size());

					for (uint32_t DefineIndex = 0; DefineIndex < MacrosCount; ++DefineIndex)
					{
						Macros.push_back({});
						DxcDefine& CurrentMacro = Macros.back();

						InDefinesUTF8[DefineIndex * 2]		= wstring(InDefines[DefineIndex * 2].begin(), InDefines[DefineIndex * 2].end());
						InDefinesUTF8[DefineIndex * 2 + 1]	= wstring(InDefines[DefineIndex * 2 + 1].begin(), InDefines[DefineIndex * 2 + 1].end());

						CurrentMacro.Name	= InDefinesUTF8[DefineIndex * 2].c_str();
						CurrentMacro.Value	= InDefinesUTF8[DefineIndex * 2 + 1].c_str();
					}

					DxcDefine PlatformMacro;
					PlatformMacro.Name	= L"PLATFORM_DX12";
					PlatformMacro.Value	= L"1";
					Macros.push_back(PlatformMacro);
				}
				wstring FileNameUTF8(InFileName.begin(), InFileName.end());
				IDxcCompilerArgs* CompilationArguments = nullptr;
				_DxcUtils->BuildArguments(
					FileNameUTF8.c_str(),
					D3D12_SHADER_ENTRIES_DXC[InShaderStageInt],
					D3D12_SHADER_PROFILES_DXC[InShaderStageInt],
					Arguments.data(),
					static_cast<UINT32>(Arguments.size()),
					Macros.data(),
					static_cast<UINT32>(Macros.size()),
					&CompilationArguments
				);

				IDxcBlobEncoding* Encoding = nullptr;
				_DxcUtils->CreateBlob(
					ShaderFileContent.data(),
					static_cast<UINT>(ShaderFileContent.size()),
					CP_ACP,
					&Encoding
				);

				DxcBuffer SourceBuffer;
				SourceBuffer.Ptr		= Encoding->GetBufferPointer();
				SourceBuffer.Size		= Encoding->GetBufferSize();
				SourceBuffer.Encoding	= 0;

				_DxcIncludeHandler->SetCurrentShader(this);

				IDxcResult* CompilationResult = nullptr;
				VerifySuccess(
					_DxcCompiler->Compile(
						&SourceBuffer,
						CompilationArguments->GetArguments(),
						CompilationArguments->GetCount(),
						_DxcIncludeHandler,
						__uuidof(IDxcResult),
						reinterpret_cast<void**>(&CompilationResult)
					)
				);

				IDxcBlobUtf8* Errors = nullptr;
				IDxcBlobUtf8* HLSLBlob = nullptr;
				IDxcBlobUtf16* OutputName = nullptr;

				VerifySuccess(
					CompilationResult->GetOutput(
						DXC_OUT_ERRORS,
						__uuidof(IDxcBlobUtf8),
						reinterpret_cast<void**>(&Errors),
						&OutputName
					)
				);

				//VerifySuccess(
				//	CompilationResult->GetOutput(
				//		DXC_OUT_HLSL,
				//		__uuidof(IDxcBlobUtf8),
				//		reinterpret_cast<void**>(&HLSLBlob),
				//		&OutputName
				//	)
				//);

				if (Errors && Errors->GetStringLength() > 0)
				{
					const char* Error = static_cast<const char*>(Errors->GetBufferPointer());
					OutputDebugString(Error);
					ETERNAL_BREAK();
				}

				CompilationResult->GetOutput(
					DXC_OUT_OBJECT,
					__uuidof(IDxcBlob),
					reinterpret_cast<void**>(&_DxcProgram),
					&OutputName
				);
			} break;
			}
		}

		void D3D12Shader::_LoadFile(_In_ const string& ShaderFile)
		{
			ETERNAL_BREAK();
		}

		bool D3D12Shader::IsShaderCompiled() const
		{
			return _FxcProgram && _DxcProgram;
		}

		template<typename ShaderBlobType>
		D3D12_SHADER_BYTECODE D3D12Shader_GetD3D12Shader(_In_ ShaderBlobType* InShaderProgram)
		{
			D3D12_SHADER_BYTECODE OutShaderByteCode;
			OutShaderByteCode.pShaderBytecode	= InShaderProgram->GetBufferPointer();
			OutShaderByteCode.BytecodeLength	= InShaderProgram->GetBufferSize();
			return OutShaderByteCode;
		}

		void D3D12Shader::GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode)
		{
			OutShaderByteCode = ShaderCompiler == ShaderCompilerType::SHADER_COMPILER_TYPE_DXC
				? D3D12Shader_GetD3D12Shader(_DxcProgram)
				: D3D12Shader_GetD3D12Shader(_FxcProgram);
		}
	}
}
