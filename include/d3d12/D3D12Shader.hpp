#pragma once

#include <string>
#include <d3d12.h>
#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class ShaderType;
		class GraphicsContext;

		class D3D12Shader final : public Shader
		{
			class D3D12Include : public ID3DInclude
			{
			public:
				virtual STDMETHODIMP Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;
				virtual STDMETHODIMP Close(THIS_ LPCVOID pData) override;
			};

		public:
			static ID3DInclude* GetIncludeHandler() { return _IncludeHandler; }

			D3D12Shader(_In_ GraphicsContext& Context, const ShaderCreateInformation& CreateInformation);
			virtual ~D3D12Shader();
			void GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode);

		protected:
			static ID3DInclude* _IncludeHandler;
			ID3DBlob* _Program;

			void _CompileFile(_In_ const string& Source, _In_ const char* Entry, _In_ const char* Profile, _In_ const vector<string>& Defines);
			void _LoadFile(const string& ShaderFile);

		private:
			D3D12Shader(_In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines = vector<string>());
		};
	}
}
