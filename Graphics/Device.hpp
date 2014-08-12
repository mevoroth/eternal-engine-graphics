#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

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
			Device(DeviceType* device);
			static DeviceType* get();
		};
	}
}

#endif
