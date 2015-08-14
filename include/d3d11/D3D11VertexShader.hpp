#ifndef _D3D11_VERTEX_SHADER_HPP_
#define _D3D11_VERTEX_SHADER_HPP_

#include "D3D11Shader.hpp"
#include "D3D11InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11VertexShader : public D3D11Shader
		{
		public:
			D3D11VertexShader(_In_ const string& Name, _In_ const string& Src, _In_ ID3D11ClassLinkage* ClassLinkage, _In_ D3D11InputLayout* InputLayout);
			virtual ~D3D11VertexShader() override;

			virtual void* GetD3D11Shader() override;

			inline D3D11InputLayout* GetInputLayout()
			{
				return _InputLayout;
			}

		private:
			ID3D11VertexShader* _Shader = nullptr;
			D3D11InputLayout* _InputLayout = nullptr;
		};
	}
}

#endif
