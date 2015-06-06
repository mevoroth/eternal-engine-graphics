#ifndef _D3D11_SHADER_FACTORY_HPP_
#define _D3D11_SHADER_FACTORY_HPP_

#include "Graphics/ShaderFactory.hpp"

struct ID3D11ClassLinkage;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11ShaderFactory : public ShaderFactory
		{
		public:
			D3D11ShaderFactory();

			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size) override;
			virtual Shader* CreateGeometryShader(_In_ const string& Name, _In_ const string& Src) override;
			virtual Shader* CreatePixelShader(_In_ const string& Name, _In_ const string& Src) override;

		private:
			ID3D11ClassLinkage* _ClassLinkage = nullptr;
		};
	}
}

#endif
