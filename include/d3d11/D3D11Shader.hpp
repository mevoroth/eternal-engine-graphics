#ifndef _D3D11_SHADER_HPP_
#define _D3D11_SHADER_HPP_

#include <d3d11.h>
#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Shader : public Shader
		{
			class D3D11Include : public ID3DInclude
			{
			public:
				virtual STDMETHODIMP Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;
				virtual STDMETHODIMP Close(THIS_ LPCVOID pData) override;
			};
		protected:
			static ID3DInclude* _IncludeHandler;
			ID3DBlob* _Program;
			ID3D11ClassLinkage* _DynamicParams;
			void _CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile);
			void _LoadFile(const string& Src);
		public:
			D3D11Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile);
			virtual void* GetD3D11Shader() = 0;
		};
	}
}

#endif
