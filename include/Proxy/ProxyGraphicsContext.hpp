#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "OutputDevice/NullOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct ProxyGraphicsContextCreateInformation : public GraphicsContextCreateInformation
		{
			ProxyGraphicsContextCreateInformation()
				: GraphicsContextCreateInformation(RenderSettings(
					DeviceType::DEVICE_TYPE_PROXY,
					1, 1
				))
			{
			}
		};

		class ProxyGraphicsContext final : public GraphicsContext
		{
		public:

			ProxyGraphicsContext()
				: GraphicsContext(
					ProxyGraphicsContextCreateInformation(),
					_NullOutputDevice
				)
			{
			}

		private:
			NullOutputDevice _NullOutputDevice;

		};
	}
}
