#ifndef _D3D11_DEVICE_HPP_
#define _D3D11_DEVICE_HPP_

#include <string>
#include <Windows.h>

#include "Graphics\Device.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Device
		{
		private:
			HWND _windowHandle;
			string _windowName;

			inline LPCTSTR _getWindowName() const;
		public:
			D3D11Device(const string& name);
			void create();
		};
		typedef Device<D3D11Device> D3D11DeviceType;
	}
}

#endif
