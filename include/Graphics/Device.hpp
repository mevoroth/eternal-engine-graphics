#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

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
			//Device();
			static Device* Get();

			virtual HWND GetWindow() const = 0;
		};
	}
}

#endif
