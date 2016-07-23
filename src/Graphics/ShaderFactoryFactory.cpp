#include "Graphics/ShaderFactoryFactory.hpp"

#include "d3d11/D3D11ShaderFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ShaderFactory* CreateShaderFactory(const ShaderFactoryType& ShaderFactoryTypeObj)
		{
			if (ShaderFactoryTypeObj == ShaderFactoryType::SHADER_FACTORY_D3D11)
				return new D3D11ShaderFactory();
		}
	}
}
