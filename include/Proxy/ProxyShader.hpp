#pragma once

#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class ProxyShader final : public Shader
		{
		public:

			ProxyShader(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation)
				: Shader(InContext, InCreateInformation)
			{
			}

			virtual bool IsShaderCompiled() const override final { return true; }
		};
	}
}
