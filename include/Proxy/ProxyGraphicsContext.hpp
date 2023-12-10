#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "OutputDevice/NullOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Shader;
		struct ShaderCreateInformation;

		struct ProxyGraphicsContextCreateInformation : public GraphicsContextCreateInformation
		{
			ProxyGraphicsContextCreateInformation(_In_ const DeviceType& InDeviceType)
				: GraphicsContextCreateInformation(RenderSettings(
					DeviceType::DEVICE_TYPE_PROXY,
					1, 1
				))
				, ProxyDeviceType(InDeviceType)
			{
			}

			DeviceType ProxyDeviceType = DeviceType::DEVICE_TYPE_NULL;
		};

		class ProxyGraphicsContext final : public GraphicsContext
		{
		public:

			ProxyGraphicsContext(_In_ const ProxyGraphicsContextCreateInformation& InProxyGraphicsContextCreateInformation)
				: GraphicsContext(
					InProxyGraphicsContextCreateInformation,
					_NullOutputDevice
				)
				, _ProxyGraphicsContextCreateInformation(InProxyGraphicsContextCreateInformation)
			{
			}

			const DeviceType& GetProxyDeviceType() const { return _ProxyGraphicsContextCreateInformation.ProxyDeviceType; }
			Shader* ResolveProxyShader(_In_ const ShaderCreateInformation& InShaderCreateInformation);

		private:
			NullOutputDevice						_NullOutputDevice;
			ProxyGraphicsContextCreateInformation	_ProxyGraphicsContextCreateInformation;

		};
	}
}
