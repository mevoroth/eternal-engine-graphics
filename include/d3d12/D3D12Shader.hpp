#pragma once

#include "Graphics/Shader.hpp"

#if ETERNAL_ENABLE_D3D12

#include <string>
#include "d3d12/D3D12Library.h"

struct ID3DInclude;
struct IDxcUtils;
struct IDxcIncludeHandler;
struct IDxcCompiler3;
struct IDxcPdbUtils;
struct IDxcBlob;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class ShaderType;
		class GraphicsContext;
		struct D3D12IncludeFXC;
		struct D3D12IncludeDXC;

		class D3D12Shader final : public Shader
		{
		public:
			static void Initialize(_Inout_ GraphicsContext& InOutContext);
			static void Destroy();
			
			D3D12Shader(_In_ GraphicsContext& InOutContext, const ShaderCreateInformation& InCreateInformation);
			D3D12Shader();
			virtual ~D3D12Shader() override final;
			virtual bool IsShaderCompiled() const override final;
			virtual void SerializeShader(_Inout_ File* InOutFile) override final;
			void GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode);
			const wchar_t* GetD3D12StageEntryPoint() const;
			const wchar_t** GetD3D12StageEntryPointPointer() const;

		protected:
#if ETERNAL_PLATFORM_WINDOWS
			static D3D12IncludeFXC*		_FxcIncludeHandler;
#endif
			static D3D12IncludeDXC*		_DxcIncludeHandler;
			static IDxcIncludeHandler*	_DxcIncludeHandlerDefault;
			static IDxcUtils*			_DxcUtils;
			static IDxcCompiler3*		_DxcCompiler3;
			static IDxcPdbUtils*		_DxcPdbUtils;

			union
			{
#if ETERNAL_PLATFORM_WINDOWS
				ID3DBlob* _FxcProgram;
#endif
				IDxcBlob* _DxcProgram;
			};

			void _CompileFile(_In_ const string& InSource, _In_ uint32_t InShaderStageInt, _In_ const vector<string>& InDefines);
			void _LoadFile(_In_ const string& InShaderFile);
		};
	}
}

#endif
