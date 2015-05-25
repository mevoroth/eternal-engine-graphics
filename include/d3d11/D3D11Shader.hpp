#ifndef _D3D11_SHADER_HPP_
#define _D3D11_SHADER_HPP_

#include <d3d11.h>
#include "Graphics/Shader.hpp"
#include "D3D11Material.hpp"

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
			ID3DBlob* _program;
			ID3D11ClassLinkage* _dynamicParams;
			void _CompileFile(_In_ const string& src, _In_ const string& entry, _In_ const string& profile);
			void _LoadFile(const string& src);
		public:
			D3D11Shader(const string& name, _In_ const string& src, _In_ const string& entry, _In_ const string& profile);
			virtual void InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code) = 0;
		};
	}
}

#endif
