#ifndef _SHADER_FACTORY_FACTORY_HPP_
#define _SHADER_FACTORY_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class ShaderFactory;

		enum ShaderFactoryType
		{
			SHADER_FACTORY_D3D11
		};

		ShaderFactory* CreateShaderFactory(const ShaderFactoryType& ShaderFactoryTypeObj);
	}
}

#endif
