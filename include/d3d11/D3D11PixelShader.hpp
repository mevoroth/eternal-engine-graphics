#ifndef _D3D11_PIXEL_SHADER_HPP_
#define _D3D11_PIXEL_SHADER_HPP_

#include "D3D11Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11PixelShader : public D3D11Shader
		{
		public:
			D3D11PixelShader(_In_ const string& name, _In_ const string& src);
			void InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code);
		};
	}
}

#endif
