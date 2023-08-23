#pragma once

#include <string>
#include <d3d12.h>
#include "Graphics/Shader.hpp"

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
			virtual ~D3D12Shader() override final;
			virtual bool IsShaderCompiled() const override final;
			void GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode);
			const wchar_t* GetD3D12StageEntryPoint() const;
			const wchar_t** GetD3D12StageEntryPointPointer() const;

		protected:
			static D3D12IncludeFXC*		_FxcIncludeHandler;
			static D3D12IncludeDXC*		_DxcIncludeHandler;
			static IDxcIncludeHandler*	_DxcIncludeHandlerDefault;
			static IDxcUtils*			_DxcUtils;
			static IDxcCompiler3*		_DxcCompiler3;
			static IDxcPdbUtils*		_DxcPdbUtils;

			union
			{
				ID3DBlob* _FxcProgram;
				IDxcBlob* _DxcProgram;
			};

			void _CompileFile(_In_ const string& InSource, _In_ uint32_t InShaderStageInt, _In_ const vector<string>& InDefines);
			void _LoadFile(_In_ const string& InShaderFile);
		};
	}
}
