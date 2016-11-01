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
			D3D11PixelShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines, _In_ ID3D11ClassLinkage* ClassLinkage);
			virtual ~D3D11PixelShader() override;

			virtual void* GetD3D11Shader() override;

		private:
			ID3D11PixelShader* _Shader = nullptr;
		};
	}
}

#endif
