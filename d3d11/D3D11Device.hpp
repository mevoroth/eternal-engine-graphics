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
			HINSTANCE _hInstance;
			int _nCmdShow;
			string _className;
			string _windowName;

			inline LPCTSTR _getClassName() const;
			inline LPCTSTR _getWindowName() const;
		public:
			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			D3D11Device(HINSTANCE hInstance, int nCmdShow, const string& name, const string& className);
			void create();
		};
		typedef Device<D3D11Device> D3D11DeviceType;
	}
}

#endif
