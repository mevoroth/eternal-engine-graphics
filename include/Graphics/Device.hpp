#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device
		{
		private:
			static Device* _Inst;

		public:
			enum
			{
				WIDTH	= 1600,
				HEIGHT	= 900
			};
			Device();
			static Device* Get();
		};
	}
}

#endif
