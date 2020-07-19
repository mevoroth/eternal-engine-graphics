#ifndef _D3D11_SHADER_HPP_
#define _D3D11_SHADER_HPP_

#include <string>
#include <d3d11.h>
#include "Graphics/Shader.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Shader : public Shader
		{
		public:
			class D3D11Include : public ID3DInclude
			{
				friend class D3D11Shader;
			public:
				struct Dependency
				{
					string FileName;
					vector<string> ShadersToRecompile;
				};
				D3D11Include();
				virtual STDMETHODIMP Open(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;
				virtual STDMETHODIMP Close(THIS_ LPCVOID pData) override;
				void RegisterShaderPath(_In_ const string& Path);
				void CreateDependency(_In_ const string& Src);
				vector<string>& GetShaderPaths();

			private:
				vector<string> _ShaderPaths;
				vector<Dependency> _ShaderDependencies;
			};

			static inline D3D11Include* GetIncludeHandler()
			{
				return (D3D11Include*)_IncludeHandler;
			}

			D3D11Shader(_In_ const string& Name, _In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile, _In_ const vector<string>& Defines);
			virtual ~D3D11Shader();
			virtual void* GetD3D11Shader() = 0;

		protected:
			static ID3DInclude* _IncludeHandler;
			ID3DBlob* _Program;

			void _CompileFile(_In_ const string& Src, _In_ const string& Entry, _In_ const string& Profile, _In_ const vector<string>& Defines);
			void _LoadFile(const string& Src);
		};
	}
}

#endif
