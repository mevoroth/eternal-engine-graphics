#include "Proxy/ProxyShader.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ProxyShader::ProxyShader()
			: Shader()
		{
		}
		ProxyShader::ProxyShader(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation)
			: Shader(InContext, InCreateInformation)
			, _Shader(static_cast<ProxyGraphicsContext&>(InContext).ResolveProxyShader(InCreateInformation))
		{
		}
	}
}
