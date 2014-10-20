#ifndef _D3D10_GEOMETRY_SHADER_HPP_
#define _D3D10_GEOMETRY_SHADER_HPP_

#include "D3D10Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D10GeometryShader : public D3D10Shader
		{
		public:
			D3D10GeometryShader(_In_ const string& name, _In_ const string& src);
			void InstantiateShader(_Out_ void** code);
		};
	}
}

#endif
