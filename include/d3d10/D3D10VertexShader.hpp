#ifndef _D3D10_VERTEX_SHADER_HPP_
#define _D3D10_VERTEX_SHADER_HPP_

#include "D3D10Shader.hpp"
#include "D3D10InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D10VertexShader : public D3D10Shader
		{
		public:
			D3D10VertexShader(_In_ const string& name, _In_ const string& src, _Inout_ D3D10InputLayout& inputLayout);
			void InstantiateShader(_Out_ void** code);
		};
	}
}

#endif
