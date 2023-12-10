#include "Proxy/ProxyGraphicsContext.hpp"
#include "Graphics/ShaderFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Shader* ProxyGraphicsContext::ResolveProxyShader(_In_ const ShaderCreateInformation& InShaderCreateInformation)
		{
			return GetShaderFactory()->GetShader(GetProxyDeviceType(), *this, InShaderCreateInformation);
		}
	}
}
