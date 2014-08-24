#ifndef _D3D11_SHADER_FACTORY_HPP_
#define _D3D11_SHADER_FACTORY_HPP_

#include <string>
#include <d3dcommon.h>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11ShaderFactory
		{
		private:
			static D3D11ShaderFactory* _inst;
			void _loadFile(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void _compileFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob);
			inline void _loadShaderFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob);

			D3D11ShaderFactory();

		public:
			static D3D11ShaderFactory* get();
			void loadVertex(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void loadGeometry(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void loadPixel(_In_ const string& shader, _Out_ ID3DBlob** blob);
		};
	}
}

#endif
