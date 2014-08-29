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
			void _LoadFile(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void _CompileFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob);
			inline void _LoadShaderFile(_In_ const string& shader, _In_ const string& profile, _Out_ ID3DBlob** blob);

			D3D11ShaderFactory();

		public:
			static D3D11ShaderFactory* Get();
			void LoadVertex(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void LoadGeometry(_In_ const string& shader, _Out_ ID3DBlob** blob);
			void LoadPixel(_In_ const string& shader, _Out_ ID3DBlob** blob);
		};
	}
}

#endif
