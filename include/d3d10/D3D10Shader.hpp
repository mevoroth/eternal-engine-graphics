#ifndef _D3D10_SHADER_HPP_
#define _D3D10_SHADER_HPP_

#include <d3d10.h>
#include "Graphics/Shader.hpp"
#include "D3D10Material.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D10Shader : public Shader
		{
		protected:
			ID3DBlob* _program;
			void _CompileFile(_In_ const string& src, _In_ const string& entry, _In_ const string& profile);
			void _LoadFile(const string& src);
		public:
			D3D10Shader(const string& name, _In_ const string& src, _In_ const string& entry, _In_ const string& profile);
			virtual void InstantiateShader(_Out_ void** code) = 0;
		};
	}
}

#endif
