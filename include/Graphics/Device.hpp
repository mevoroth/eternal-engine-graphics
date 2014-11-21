#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include <cassert>

namespace Eternal
{
	namespace Graphics
	{
		class Device
		{
		private:
			static Device* _inst;

		public:
			enum
			{
				WIDTH	= 640,//1280,
				HEIGHT	= 480//720
			};
			Device();
			static Device* Get();
		};
	}
}

#endif
