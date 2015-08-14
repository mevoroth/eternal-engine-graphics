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
		class D3D11Device : public Device
		{
		private:
			HWND _windowHandle;
			HINSTANCE _hInstance;
			int _nCmdShow;
			string _ClassName;
			string _windowName;

			inline LPCTSTR _GetClassName() const;
			inline LPCTSTR _GetWindowName() const;
		public:
			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			D3D11Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& name, _In_ const string& className);
			void Create();
			HWND GetWindow() const;
		};
	}
}

#endif
