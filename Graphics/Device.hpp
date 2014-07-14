#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device
		{
		private:
			static Device* _inst;
			Device();

		public:
			const int WIDTH = 1280;
			const int HEIGHT = 720;
			static Device* get();
		};
	}
}

#endif
