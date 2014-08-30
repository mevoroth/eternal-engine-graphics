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
			D3D11VertexShader(_In_ const string& name, _In_ const string& src, _Inout_ D3D11InputLayout& inputLayout);
			void InstantiateShader(_In_ ID3D11ClassLinkage* classLinkage, _Out_ void** code);
		};
	}
}

#endif
