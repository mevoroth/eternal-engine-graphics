#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include <cassert>

namespace Eternal
{
	namespace Graphics
	{
		template <class DeviceType> class Device
		{
		private:
			static DeviceType* _inst;

		public:
			enum
			{
				WIDTH	= 1280,
				HEIGHT	= 720
			};
			Device(_In_ DeviceType* device);
			static DeviceType* Get();
		};

		template <class DeviceType> DeviceType* Device<DeviceType>::_inst = 0;

		template <class DeviceType> Device<DeviceType>::Device(_In_ DeviceType* device)
		{
			assert(!_inst);
			_inst = device;
		}

		template <class DeviceType> DeviceType* Device<DeviceType>::Get()
		{
			return _inst;
		}
	}
}

#endif
