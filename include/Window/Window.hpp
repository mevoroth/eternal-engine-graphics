#pragma once

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

		struct WindowCreateInformation
		{
			WindowCreateInformation(
				_In_ HINSTANCE hInInstance,
				_In_ int nInCmdShow,
				_In_ const string& InName,
				_In_ const string& InClassName,
				_In_ int InWidth,
				_In_ int InHeight,
				_In_ bool InVSync = false,
				_In_ bool InWindowed = true
			)
				: ClassName(InClassName)
				, WindowName(InName)
				, hInstance(hInInstance)
				, nCmdShow(nInCmdShow)
				, Width(InWidth)
				, Height(InHeight)
				, VSync(InVSync)
				, Windowed(InWindowed)
			{

			}
			string ClassName;
			string WindowName;
			HINSTANCE hInstance	= nullptr;
			int nCmdShow		= 0;
			int Width			= 640;
			int Height			= 480;
			bool VSync			= false;
			bool Windowed		= true;
		};

		class Window
		{
		public:
			Window(_In_ const WindowCreateInformation& InWindowCreateInformation);

			void Create(WNDPROC WindowEventsHandler);
			HWND GetWindowHandler() const { return _WindowHandle; }
			HINSTANCE GetHInstance() const { return _WindowCreateInformation.hInstance; }
			
			inline bool GetVSync() const { return _WindowCreateInformation.VSync; }
			inline bool GetWindowed() const { return _WindowCreateInformation.Windowed; }
			inline int GetWidth() const { return _WindowCreateInformation.Width; }
			inline int GetHeight() const { return _WindowCreateInformation.Height; }
			inline int GetNCmdShow() const { return _WindowCreateInformation.nCmdShow; }
			inline const string& GetClassName() const { return _WindowCreateInformation.ClassName; }
			inline const string& GetWindowName() const { return _WindowCreateInformation.WindowName; }

		private:
			WindowCreateInformation _WindowCreateInformation;
			HWND _WindowHandle = nullptr;
		};
	}
}
