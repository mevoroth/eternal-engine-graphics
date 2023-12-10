#pragma once

#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;

		class ProxyDevice final : public Device
		{
		public:

			ProxyDevice(_Inout_ GraphicsContext& InOutContext);
			~ProxyDevice();

			virtual uint32_t GetDeviceMask() const override final { return 0x1; };
			virtual DeviceType GetDeviceType() const override final { return DeviceType::DEVICE_TYPE_PROXY; };

		private:

			GraphicsContext& _Context;

		};
	}
}
