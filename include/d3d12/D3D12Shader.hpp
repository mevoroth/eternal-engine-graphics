#ifndef _D3D12_SHADER_HPP_
#define _D3D12_SHADER_HPP_

#include <d3d12.h>
#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Shader : public Shader
		{
			class D3D12Include : public ID3DInclude
			{
			public:
				virtual STDMETHODIMP Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;
				virtual STDMETHODIMP Close(THIS_ LPCVOID pData) override;
			};

		public:
			static ID3DInclude* GetIncludeHandler() { return _IncludeHandler; }

			D3D12Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile);
			virtual ~D3D12Shader();
			void GetD3D12Shader(_Out_ D3D12_SHADER_BYTECODE& ShaderObj);

		protected:
			static ID3DInclude* _IncludeHandler;
			ID3DBlob* _Program;

			void _CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile);
			void _LoadFile(const string& Src);
		};
	}
}

#endif
