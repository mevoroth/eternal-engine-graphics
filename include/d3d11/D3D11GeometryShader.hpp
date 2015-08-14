#ifndef _D3D11_GEOMETRY_SHADER_HPP_
#define _D3D11_GEOMETRY_SHADER_HPP_

#include "D3D11Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11GeometryShader : public D3D11Shader
		{
		public:
			D3D11GeometryShader(_In_ const string& Name, _In_ const string& Src, _In_ ID3D11ClassLinkage* ClassLinkage);
			virtual ~D3D11GeometryShader() override;

			virtual void* GetD3D11Shader() override;

		private:
			ID3D11GeometryShader* _Shader = nullptr;
		};
	}
}

#endif
