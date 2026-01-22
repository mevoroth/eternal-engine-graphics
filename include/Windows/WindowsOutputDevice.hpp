#pragma once

#if ETERNAL_PLATFORM_WINDOWS

#include "OutputDevice/OutputDevice.hpp"

#include <string>
#include <Windows.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		struct WindowsOutputDeviceCreateInformation : public OutputDeviceCreateInformation
		{
			WindowsOutputDeviceCreateInformation(
				_In_ HINSTANCE hInInstance,
				_In_ int nInCmdShow,
				_In_ const string& InName,
				_In_ const string& InClassName,
				_In_ int InWidth,
				_In_ int InHeight
			)
				: OutputDeviceCreateInformation(
					InWidth,
					InHeight
				)
				, ClassName(InClassName)
				, WindowName(InName)
				, hInstance(hInInstance)
				, nCmdShow(nInCmdShow)
			{
			}

			string ClassName;
			string WindowName;
			HINSTANCE hInstance	= nullptr;
			int nCmdShow		= 0;
		};


		class WindowsOutputDevice : public OutputDevice
		{
		public:

			WindowsOutputDevice(_In_ WindowsOutputDeviceCreateInformation& InOutputDeviceCreateInformation);

			void Create(_In_ WNDPROC InWindowEventsHandler);
			HWND GetWindowHandler() const { return _WindowHandle; }
			HINSTANCE GetHInstance() const { return _hInstance; }

			inline bool GetWindowed() const { return _OutputDeviceCreateInformation.Windowed; }
			inline int GetNCmdShow() const { return _nCmdShow; }
			inline const string& GetWindowClassName() const { return _ClassName; }
			inline const string& GetWindowName() const { return _WindowName; }

			virtual const char* GetApplicationName() const;

		private:

			string _ClassName;
			string _WindowName;
			HINSTANCE _hInstance	= nullptr;
			int _nCmdShow			= 0;
			HWND _WindowHandle		= nullptr;
		};
	}
}

#endif
