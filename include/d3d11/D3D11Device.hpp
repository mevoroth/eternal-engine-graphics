#ifndef _D3D11_DEVICE_HPP_
#define _D3D11_DEVICE_HPP_

#include <string>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include "Graphics_deprecated/Device.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Device : public Device
		{
		private:
			HWND _WindowHandle;
			HINSTANCE _hInstance;
			int _nCmdShow;
			string _ClassName;
			string _WindowName;

			inline LPCTSTR _GetClassName() const;
			inline LPCTSTR _GetWindowName() const;
		public:
			D3D11Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& name, _In_ const string& className);
			void Create(WNDPROC WindowEventsHandler);
			virtual HWND GetWindow() const override;
		};
	}
}

#endif
