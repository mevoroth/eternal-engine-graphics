#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <string>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#undef GetClassName

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
			HINSTANCE GetHInstance() const;
			
			inline bool GetWindowed() const { return _Windowed; }
			inline int GetWidth() const { return _Width; }
			inline int GetHeight() const { return _Height; }
			inline const string& GetClassName() const { return _ClassName; }
			inline const string& GetWindowName() const { return _WindowName; }

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
