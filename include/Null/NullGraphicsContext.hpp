#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "OutputDevice/NullOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct NullGraphicsContextCreateInformation : public GraphicsContextCreateInformation
		{
			NullGraphicsContextCreateInformation()
				: GraphicsContextCreateInformation(RenderSettings(
					DeviceType::DEVICE_TYPE_NULL,
					1, 1
				))
			{
			}
		};

		class NullGraphicsContext final : public GraphicsContext
		{
		public:

			NullGraphicsContext()
				: GraphicsContext(
					NullGraphicsContextCreateInformation(),
					_NullOutputDevice
				)
			{
			}

		private:
			NullOutputDevice _NullOutputDevice;

		};
	}
}
