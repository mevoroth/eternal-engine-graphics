#pragma once

#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class ProxyShader final : public Shader
		{
		public:

			ProxyShader();
			ProxyShader(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation);

			virtual bool IsShaderCompiled() const override final { return _Shader->IsShaderCompiled(); }
			virtual void SerializeShader(_Inout_ File* InOutFile) override final { _Shader->SerializeShader(InOutFile); }

		private:

			Shader* _Shader = nullptr;

		};
	}
}
