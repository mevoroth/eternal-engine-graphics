#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <string>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Window
		{
		public:
			Window(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName, _In_ int Width, _In_ int Height);

			void Create(WNDPROC WindowEventsHandler);
			HWND GetWindowHandler() const;
			
			inline bool GetWindowed() const { return _Windowed; }
			inline int GetWidth() const { return _Width; }
			inline int GetHeight() const { return _Height; }

		private:
			HWND _WindowHandle;
			HINSTANCE _hInstance;
			int _nCmdShow;
			string _ClassName;
			string _WindowName;
			int _Width = 640;
			int _Height = 480;
			bool _Windowed = false;
		};
	}
}

#endif
